/*
    SqliteHelper 类
*/
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <string>

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
            std::cout << "创建/打开sqlite数据库失败: ";
            std::cout << sqlite3_errmsg(_handler) << std::endl;
            return false;
        }
        return true;
    }

    bool exec(const std::string &sql, SqliteCallback cb, void *arg)
    {
        int ret = sqlite3_exec(_handler, sql.c_str(), cb, arg, nullptr);
        if (ret != SQLITE_OK)
        {
            std::cout << sql;
            std::cout << "执行语句失败:";
            std::cout << sqlite3_errmsg(_handler) << std::endl;
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
