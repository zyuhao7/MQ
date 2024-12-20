#include "../common/logger.hpp"
#include "../common/helper.hpp"
#include "../common/msg.pb.h"
#include <unordered_map>
#include <iostream>
#include <mutex>
#include <memory>

namespace mq
{
    struct MsgQueue
    {
        std::string name;
        bool durable;
        bool exclusive;
        bool auto_delete;
        std::unordered_map<std::string, std::string> args;
        using ptr = std::shared_ptr<MsgQueue>;
        MsgQueue() {}
        MsgQueue(const std::string &qname, bool qdurable, bool qexclusive, bool qauto_delete, std::unordered_map<std::string, std::string> &qargs)
            : name(qname),
              durable(qdurable),
              exclusive(qexclusive),
              auto_delete(qauto_delete),
              args(qargs)
        {
        }

        void setArgs(const std::string &str_args)
        {
            std::vector<std::string> sub_args;
            StrHelper::split(str_args, "&", sub_args);
            for (auto &str : sub_args)
            {
                size_t pos = str.find("=");
                std::string key = str.substr(0, pos);
                std::string val = str.substr(pos + 1);
                args[key] = val;
            }
        }
        // 将args的内容进行序列化后返回一个字符串.
        std::string getArgs()
        {
            std::string result;
            for (auto it = args.begin(); it != args.end(); it++)
            {
                result += it->first + "=" + it->second + "&";
            }
            return result;
            // result.pop_back();
        }
    };

    class MsgQueueMapper
    {
    public:
        MsgQueueMapper(const std::string &dbfile)
            : _sql_helper(dbfile)
        {
            std::string path = FileHelper::parentDirectory(dbfile);
            FileHelper::createDirectory(path);
            if (_sql_helper.open() == false)
            {
                LOG_ERROR("打开数据库失败!");
            }
            createTable();
        }

        void createTable()
        {
            std::stringstream sql;
            sql << "create table if not exists queue_table(";
            sql << "name varchar(32) primary key,";
            sql << "durable int, ";
            sql << " exclusive int, ";
            sql << " auto_delete int,";
            sql << "args varchar(128));";
            assert(_sql_helper.exec(sql.str(), nullptr, nullptr));
        }

        void removeTable()
        {
            std::string sql = "drop table if exists queue_table;";
            assert(_sql_helper.exec(sql, nullptr, nullptr));
        }

        bool insert(MsgQueue::ptr queue)
        {
            std::stringstream sql;
            sql << "insert into queue_table values(";
            sql << "'" << queue->name << "', ";
            sql << queue->durable << ", ";
            sql << queue->exclusive << ", ";
            sql << queue->auto_delete << ", ";
            sql << "'" << queue->getArgs() << "');";

            return _sql_helper.exec(sql.str(), nullptr, nullptr);
        }

        void remove(const std::string &name)
        {
            std::stringstream sql;
            sql << "delete from queue_table where name=";
            sql << "'" << name << "';";
            _sql_helper.exec(sql.str(), nullptr, nullptr);
        }
        using QueueMap = std::unordered_map<std::string, MsgQueue::ptr>;
        QueueMap recovery()
        {
            QueueMap result;
            std::string sql = "select name, durable, exclusive, auto_delete, args from queue_table";
            _sql_helper.exec(sql, selectCallBack, &result);
            return result;
        }

    private:
        static int selectCallBack(void *args, int numcol, char **row, char **fields)
        {
            QueueMap *result = (QueueMap *)args;
            auto queue = std::make_shared<MsgQueue>();
            queue->name = row[0];
            queue->durable = (bool)std::stoi(row[1]);
            queue->exclusive = (bool)std::stoi(row[2]);
            queue->auto_delete = (bool)std::stoi(row[3]);
            if (row[4])
                queue->setArgs(row[4]);
            result->insert(std::make_pair(queue->name, queue));
            return 0;
        }

    private:
        SqliteHelper _sql_helper;
    };
    using QueueMap = std::unordered_map<std::string, MsgQueue::ptr>;

    class MsgQueueManager
    {
    public:
        using ptr = std::shared_ptr<MsgQueueManager>;
        MsgQueueManager(const std::string &dbfile)
            : _mapper(dbfile)
        {
            _msg_queues = _mapper.recovery();
        }

        bool declareQueue(const std::string &qname, bool qdurable, bool qexclusive, bool qauto_delete, std::unordered_map<std::string, std::string> &qargs)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _msg_queues.find(qname);
            if (it != _msg_queues.end())
            {
                return true;
            }
            auto queue = std::make_shared<MsgQueue>(qname, qdurable, qexclusive, qauto_delete, qargs);
            if (qdurable == true)
            {
                bool ret = _mapper.insert(queue);
                if (ret == false)
                    return false;
            }
            _msg_queues.insert(std::make_pair(qname, queue));
            return true;
        }

        void deleteQueue(const std::string &qname)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            QueueMap::iterator it = _msg_queues.find(qname);
            if (it == _msg_queues.end())
            {
                return;
            }
            if (it->second->durable == true)
                _mapper.remove(qname);
            _msg_queues.erase(qname);
        }

        MsgQueue::ptr selectQueue(const std::string &qname)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _msg_queues.find(qname);
            if (it == _msg_queues.end())
            {
                return MsgQueue::ptr();
            }
            return it->second;
        }
        QueueMap getAllQueues()
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return _msg_queues;
        }

        bool exists(const std::string &qname)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return _msg_queues.find(qname) != _msg_queues.end();
        }

        size_t size()
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return _msg_queues.size();
        }

        void clear()
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _msg_queues.clear();
            _mapper.removeTable();
        }

    private:
        MsgQueueMapper _mapper;
        QueueMap _msg_queues;
        std::mutex _mutex;
    };
}