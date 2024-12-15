#ifndef __M_HELPER_M__
#define __M_HELPER_M__
/*
    SqliteHelper 类
*/
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <string>
#include "logger.hpp"
namespace mq
{

    class SqliteHelper
    {

    public:
        typedef int(SqliteCallback)(void *, int, char **, char **);
        SqliteHelper(const std::string &dbfile)
            : _dbfile(dbfile), _handler(nullptr) {}
        // ~SqliteHelper();

        bool open(int safe_level = SQLITE_OPEN_FULLMUTEX)
        {
            int ret = sqlite3_open_v2(_dbfile.c_str(), &_handler, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | safe_level, nullptr);
            if (ret != SQLITE_OK)
            {
                LOG_ERROR("创建/打开sqlite数据库失败:%s", sqlite3_errmsg(_handler));
                return false;
            }
            return true;
        }

        bool exec(const std::string &sql, SqliteCallback cb, void *arg)
        {
            int ret = sqlite3_exec(_handler, sql.c_str(), cb, arg, nullptr);
            if (ret != SQLITE_OK)
            {
                LOG_ERROR("%s \n, 执行语句失败: %s", sql.c_str(), sqlite3_errmsg(_handler));
                return false;
            }
            return true;
        }
        void close()
        {
            if (_handler)
                sqlite3_close_v2(_handler);
        }

    private:
        std::string _dbfile;
        sqlite3 *_handler;
    };

    class StrHelper
    {
    public:
        static size_t split(const std::string &str, const std::string &sep, std::vector<std::string> &res)
        {
            size_t pos, idx = 0;
            while (idx < str.size())
            {
                pos = str.find(sep, idx);
                if (pos == std::string::npos)
                {
                    res.push_back(str.substr(idx));
                    return res.size();
                }
                if (pos == idx)
                {
                    idx += sep.size();
                    continue;
                }
                res.push_back(str.substr(idx, pos - idx));
                idx += pos - idx;
            }
            return res.size();
        }
    };
}

#endif