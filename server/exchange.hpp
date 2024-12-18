#include "../common/logger.hpp"
#include "../common/helper.hpp"
#include "../common/msg.pb.h"
#include <unordered_map>
#include <iostream>
#include <mutex>
#include <memory>

namespace mq
{
    // 1 定义交换机
    struct Exchange
    {
        using ptr = std::shared_ptr<Exchange>;
        // 1. 交换机名称
        std::string name;
        // 2. 交换机类型
        ExchangeType type;
        // 3. 交换机持久化标志
        bool durable;
        // 4. 是否自动删除标志
        bool auto_delete;
        // 5. 其它参数
        std::unordered_map<std::string, std::string> args;
        Exchange() {}
        Exchange(const std::string ename,
                 ExchangeType etype,
                 bool edurable,
                 bool eauto_delete,
                 std::unordered_map<std::string, std::string> &eargs)
            : name(ename),
              type(etype),
              durable(edurable),
              auto_delete(eauto_delete),
              args(eargs)
        {
        }
        // args 存储键值对, 在存储数据库的时候, 会组织一个格式字符串进行存储, key=val&key=val...
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
        // bool parse();
    };

    using ExchangeMap = std::unordered_map<std::string, Exchange::ptr>;

    // 2. 定义交换机数据持久化管理类, 数据存储在 sqlite 数据库中
    class ExchangeMapper
    {
    public:
        ExchangeMapper(const std::string &dbfile)
            : _sql_helper(dbfile)
        {
            std::string path = FileHelper::parentDirectory(dbfile);
            assert(_sql_helper.open());
            createTable();
        }
        void createTable()
        {
#define CREATE_TABLE "create table if not exists exchange_table(name varchar(32) primary key,\
            type int, \
            durable int, \
             auto_delete int, \
             args varchar(128));"
            bool ret = _sql_helper.exec(CREATE_TABLE, nullptr, nullptr);
            if (ret == false)
            {
                LOG_DEBUG("创建交换机数据库失败!");
                exit(EXIT_FAILURE);
            }
        }
        void removeTable()
        {
#define DROP_TABLE "drop table if exists exchange_table;"
            bool ret = _sql_helper.exec(DROP_TABLE, nullptr, nullptr);
            if (ret == false)
            {
                LOG_DEBUG("删除交换机数据库失败!");
                exit(EXIT_FAILURE);
            }
        }
        void insert(Exchange::ptr &exp)
        {
            // #define INSERT_SQL "insert into exchange_table values('%s', %d, %d, %d, '%s')"
            // char sql_str[4096] = {0};
            // std::string args_str = exp->getArgs();
            // sprintf(sql_str, INSERT_SQL, exp->name, exp->type, exp->durable, exp->auto_delete, args_str.c_str());
            std::stringstream ss;
            ss << "insert into exchange_table values(";
            ss << "'" << exp->name << "', ";
            ss << exp->type << ", ";
            ss << exp->durable << ", ";
            ss << exp->auto_delete << ", ";
            ss << "'" << exp->getArgs() << "');";

            _sql_helper.exec(ss.str(), nullptr, nullptr);
        }
        void remove(const std::string &name)
        {
            std::stringstream ss;
            ss << "delete from exchange_table where name=";
            ss << "'" << name << "';";
            _sql_helper.exec(ss.str(), nullptr, nullptr);
            
        }

        ExchangeMap recovery()
        {
            ExchangeMap result;
            std::string sql;
            sql = "select name, type, durable, auto_delete, args from exchange_table";
            _sql_helper.exec(sql, selectCallback, &result);
            return result;
        }

    private:
        static int selectCallback(void *args, int numcol, char **row, char **fileds)
        {
            ExchangeMap *result = (ExchangeMap *)args;
            auto exp = std::make_shared<Exchange>();
            exp->name = row[0];
            exp->type = (mq::ExchangeType)std::stoi(row[1]);
            exp->durable = (bool)std::stoi(row[2]);
            exp->auto_delete = (bool)std::stoi(row[3]);
            if (row[4])
                exp->setArgs(row[4]);
            result->insert(std::make_pair(exp->name, exp));
            return 0;
        }

    private:
        SqliteHelper _sql_helper;
    };

    // 3. 定义交换机数据内存管理类
    class ExchangeManager
    {
    public:
        using ptr = std::shared_ptr<ExchangeManager>;
        ExchangeManager(const std::string &dbfile)
            : _mapper(dbfile)
        {
            _exchanges = _mapper.recovery();
        }
        // 声明交换机
        void declareExchange(const std::string &name, ExchangeType type,
                             bool durable, bool auto_delete, std::unordered_map<std::string, std::string> &args)

        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _exchanges.find(name);
            if (it != _exchanges.end())
            {
                // 交换机存在
                return;
            }
            auto exp = std::make_shared<Exchange>(name, type, durable, auto_delete, args);
            {
                if (durable == true)
                    _mapper.insert(exp);
            }
            _exchanges.insert(std::make_pair(name, exp));
        }
        // 删除交换机
        void deleteExchange(const std::string &name)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _exchanges.find(name);
            if (it == _exchanges.end())
            {
                // 交换机不存在
                return;
            }
            if (it->second->durable == true)
                _mapper.remove(name);

            _exchanges.erase(name);
        }

        // 获取指定交换机对象
        Exchange::ptr selectExchange(const std::string &name)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _exchanges.find(name);
            if (it == _exchanges.end())
            {
                // 交换机不存在
                return Exchange::ptr();
            }
            return it->second;
        }
        // 是否存在
        bool exists(const std::string &name)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _exchanges.find(name);
            if (it == _exchanges.end())
            {
                // 交换机不存在
                return false;
            }
            return true;
        }

        size_t size()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _exchanges.size();
        }

        // 清理所有交换机
        bool clear() // test
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _mapper.removeTable();
            _exchanges.clear();
            return true;
        }

    private:
        std::mutex _mutex;
        ExchangeMapper _mapper;
        std::unordered_map<std::string, Exchange::ptr> _exchanges;
    };
};
