#ifndef __M_HELPER_M__
#define __M_HELPER_M__
/*
    SqliteHelper 类
*/
#include <sqlite3.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <cerrno>
#include <string>
#include <string.h>
#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include <cerrno>
#include <atomic>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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

    class UUIDHelper
    {
    public:
        static std::string uuid()
        {

            std::random_device rd;
            std::mt19937_64 generator(rd());
            std::uniform_int_distribution<int> distribution(0, 255);
            std::stringstream ss;
            for (int i = 0; i < 8; ++i)
            {
                ss << std::setw(2) << std::setfill('0') << std::hex << distribution(generator);
                if (i == 3 || i == 5 || i == 7)
                    ss << "-";
            }
            static std::atomic<size_t> seq(1);
            size_t num = seq.fetch_add(1);
            for (int i = 7; i >= 0; --i)
            {
                ss << std::setw(2) << std::setfill('0') << std::hex << ((num >> i * 8) & 0xff);
                if (i == 6)
                    ss << "-";
            }
            return ss.str();
        }
    };
    class FileHelper
    {
    public:
        FileHelper(const std::string &filename) : _filename(filename) {}
        bool exists()
        {
            struct stat st;
            return stat(_filename.c_str(), &st) == 0;
        }

        size_t size()
        {
            struct stat st;
            int ret = stat(_filename.c_str(), &st);
            if (ret < 0)
                return 0;
            return st.st_size;
        }

        bool read(char *body, size_t offset, size_t len)
        {
            std::ifstream ifs(_filename, std::ios::binary | std::ios::in);
            if (ifs.is_open() == false)
            {
                LOG_ERROR("%s 文件打开失败!", _filename.c_str());
                return false;
            }
            ifs.seekg(offset, std::ios::beg);
            ifs.read(body, len);
            if (ifs.good() == false)
            {
                LOG_ERROR("%s 读取文件失败!", _filename.c_str());
                return false;
            }
            ifs.close();
            return true;
        }

        bool read(std::string &body)
        {
            size_t fsize = this->size();
            body.resize(fsize);
            return read(&body[0], 0, fsize);
        }

        bool write(const char *body, size_t offset, size_t len)
        {
            std::fstream fs(_filename, std::ios::binary | std::ios::in | std::ios::out);
            if (fs.is_open() == false)
            {
                LOG_ERROR("%s 文件打开失败!", _filename.c_str());
                return false;
            }

            fs.seekg(offset, std::ios::beg);
            fs.write(body, len);
            if (fs.good() == false)
            {
                LOG_ERROR("%s 写入文件失败!", _filename.c_str());
                return false;
            }
            fs.close();
            return true;
        }

        bool write(const std::string &body)
        {
            return write(body.c_str(), 0, body.size());
        }

        static std::string parentDirectory(const std::string &filename)
        {
            size_t pos = filename.find_last_of("/");
            if (pos == std::string::npos)
            {
                return "./";
            }
            std::string path = filename.substr(0, pos);
            return path;
        }

        bool rename(const std::string &newName)
        {
            return ::rename(_filename.c_str(), newName.c_str()) == 0;
        }

        static bool createFile(const std::string &filename)
        {
            std::ofstream ofs(filename, std::ios::binary | std::ios::out);
            if (ofs.is_open() == false)
            {
                LOG_ERROR("%s 文件打开失败!", filename.c_str());
                return false;
            }
            ofs.close();
            return true;
        }

        static bool removeFile(const std::string &filename)
        {
            return ::remove(filename.c_str()) == 0;
        }

        static bool createDirectory(const std::string &path)
        {
            size_t pos, idx = 0;
            while (idx < path.size())
            {
                pos = path.find("/", idx);
                if (pos == std::string::npos)
                {
                    return mkdir(path.c_str(), 0775) == 0;
                }
                std::string subpath = path.substr(0, pos);
                int ret = mkdir(subpath.c_str(), 0775);
                if (ret != 0 && errno != EEXIST)
                {
                    LOG_ERROR("创建目录 %s 失败 %s", subpath.c_str(), strerror(errno));
                    return false;
                }
                idx = pos + 1;
            }
            return true;
        }

        static bool removeDirectory(const std::string &path)
        {
            // rm -rf
            std::string cmd = "rm -rf " + path;
            return system(cmd.c_str()) != -1;
        }

    private:
        std::string _filename;
    };
}

#endif