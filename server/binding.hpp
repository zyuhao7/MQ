#include "../common/logger.hpp"
#include "../common/helper.hpp"
#include "../common/msg.pb.h"
#include <unordered_map>
#include <iostream>
#include <mutex>
#include <memory>

namespace mq
{
    struct Binding
    {
        using ptr = std::shared_ptr<Binding>;

        std::string exchange_name;
        std::string msgqueue_name;
        std::string binding_key;
        Binding() {}
        Binding(const std::string &ename,
                const std::string &mqname,
                const std::string &bkey)
            : exchange_name(ename),
              msgqueue_name(mqname),
              binding_key(bkey)
        {
        }
    };
    // 队列与绑定信息的映射, 一个队列可以绑定多个交换机, 一个交换机可以绑定多个队列.
    using msgQueueBindingMap = std::unordered_map<std::string, Binding::ptr>;
    // 交换机与绑定信息的映射, 一个交换机可以绑定多个队列, 一个队列可以绑定多个交换机.
    using BindingMap = std::unordered_map<std::string, msgQueueBindingMap>;
    class BindingMapper
    {
    public:
        BindingMapper(const std::string &dbfile)
            : _sql_helper(dbfile)
        {
            std::string path = FileHelper::parentDirectory(dbfile);
            FileHelper::createFile(path);
            _sql_helper.open();
            createTable();
        }

        void createTable()
        {
            std::stringstream sql;
            sql << "create table if not exists binding_table(";
            sql << "exchange_name varchar(32),";
            sql << "msgqueue_name varchar(32),";
            sql << "binding_key varchar(128));";
            assert(_sql_helper.exec(sql.str(), nullptr, nullptr));
        }

        void removeTable()
        {
            std::stringstream sql;
            sql << "drop table if exists binding_table;";
            assert(_sql_helper.exec(sql.str(), nullptr, nullptr));
        }

        bool insert(Binding::ptr &binding)
        {
            std::stringstream sql;
            sql << "insert into binding_table values(";
            sql << "'" << binding->exchange_name << "', ";
            sql << "'" << binding->msgqueue_name << "', ";
            sql << "'" << binding->binding_key << "');";
            return _sql_helper.exec(sql.str(), nullptr, nullptr);
        }

        void remove(const std::string &exchange_name, const std::string &msgqueue_name)
        {
            std::stringstream sql;
            sql << "delete from binding_table where exchange_name='";
            sql << exchange_name << "' and msgqueue_name='";
            sql << msgqueue_name << "';";
            _sql_helper.exec(sql.str(), nullptr, nullptr);
        }

        void removeExchangeBindings(const std::string &exchange_name)
        {
            std::stringstream sql;
            sql << "delete from binding_table where exchange_name='";
            sql << exchange_name << "';";
            _sql_helper.exec(sql.str(), nullptr, nullptr);
        }

        void removeMsgQueueBindings(const std::string &msgqueue_name)
        {
            std::stringstream sql;
            sql << "delete from binding_table where msgqueue_name='";
            sql << msgqueue_name << "';";
            _sql_helper.exec(sql.str(), nullptr, nullptr);
        }
        BindingMap recovery()
        {
            BindingMap result;
            std::string sql;
            sql = "select * from binding_table;";
            _sql_helper.exec(sql, selectCallback, &result);
            return result;
        }

    private:
        static int selectCallback(void *args, int numcol, char **row, char **fileds)
        {
            BindingMap *result = (BindingMap *)args;
            Binding::ptr bp = std::make_shared<Binding>(row[0], row[1], row[2]);
            // 如果交换机不存在, 则创建一个新的交换机
            if (result->find(row[0]) == result->end())
            {
                msgQueueBindingMap mqb;
                mqb.insert(std::make_pair(row[1], bp));
                result->insert(std::make_pair(row[0], mqb));
            }
            // 如果交换机存在, 则直接插入队列与绑定信息的映射
            else
            {
                (*result)[row[0]].insert(std::make_pair(row[1], bp));
            }
            // 如果队列不存在, 则创建一个新的队列
            (*result)[row[0]][row[1]] = bp;
            return 0;
        }

    private:
        SqliteHelper _sql_helper;
    };

    class BindManager
    {
    public:
        BindManager(const std::string &dbfile)
            : _mapper(dbfile)
        {
            _bindings = _mapper.recovery();
        }
        bool bind(const std::string &exchange_name,
                  const std::string &msgqueue_name,
                  const std::string &binding_key,
                  bool durable = false)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _bindings.find(exchange_name);
            if (it != _bindings.end() && it->second.find(msgqueue_name) != it->second.end())
                return true;

            Binding::ptr bp = std::make_shared<Binding>(exchange_name, msgqueue_name, binding_key);

            if (durable)
            {
                bool ret = _mapper.insert(bp);
                if (ret == false)
                    return false;
            }
            auto &mqb = _bindings[exchange_name];
            mqb.insert(std::make_pair(msgqueue_name, bp));
            return true;
        }
        void unBind(const std::string &exchange_name, const std::string &msgqueue_name)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _bindings.find(exchange_name);
            if (it == _bindings.end()) // 交换机不存在
                return;
            auto it2 = it->second.find(msgqueue_name);
            if (it2 == it->second.end()) // 队列不存在
                return;
            _mapper.remove(exchange_name, msgqueue_name);
            // 删除队列与绑定信息的映射
            _bindings[exchange_name].erase(msgqueue_name);
        }

        void removeExchangeBindings(const std::string &exchange_name)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _mapper.removeExchangeBindings(exchange_name);
            _bindings.erase(exchange_name);
        }

        void removeMsgQueueBindings(const std::string &msgqueue_name)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _mapper.removeMsgQueueBindings(msgqueue_name);
            for (auto it = _bindings.begin(); it != _bindings.end(); ++it)
            {
                // 删除队列与绑定信息的映射
                it->second.erase(msgqueue_name);
            }
        }

        msgQueueBindingMap getExchangeBindings(const std::string &exchange_name)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (_bindings.find(exchange_name) == _bindings.end())
                return msgQueueBindingMap();
            return _bindings[exchange_name];
        }

        Binding::ptr getBinding(const std::string &exchange_name, const std::string &msgqueue_name)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (_bindings.find(exchange_name) == _bindings.end())
                return Binding::ptr();
            if (_bindings[exchange_name].find(msgqueue_name) == _bindings[exchange_name].end())
                return Binding::ptr();
            return _bindings[exchange_name][msgqueue_name];
        }

        bool exists(const std::string &exchange_name, const std::string &msgqueue_name)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (_bindings.find(exchange_name) == _bindings.end())
                return false;
            if (_bindings[exchange_name].find(msgqueue_name) == _bindings[exchange_name].end())
                return false;
            return true;
        }

        size_t size()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            size_t total_size = 0;
            for(auto it = _bindings.begin(); it != _bindings.end(); ++it)
            {
                total_size += it->second.size();
            }
            return total_size;
        }

        void clear()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _mapper.removeTable();
            _bindings.clear();
        }

    private:
        std::mutex _mutex;
        BindingMapper _mapper;
        BindingMap _bindings;
    };
}