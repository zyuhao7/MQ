#ifndef __M_LOG_H__
#define __M_LOG_H_

#include <iostream>
#include <ctime>
// 封装一个日志宏, 通过日志宏进行日志的打印.
// [19:26:27][log.cpp:10]
namespace mq
{
#define DBG_LEVEL 0
#define INF_LEVEL 1
#define ERR_LEVEL 2
#define DEFAULT_LEVEL DBG_LEVEL
#define LOG(lev_str, level, format, ...)                                                                 \
    {                                                                                                    \
        if (level >= DEFAULT_LEVEL)                                                                      \
        {                                                                                                \
            time_t t = time(nullptr);                                                                    \
            struct tm *ptm = localtime(&t);                                                              \
            char time_str[32];                                                                           \
            strftime(time_str, 31, "%H:%M:%S", ptm);                                                     \
            printf("[%s][%s][%s:%d]" format "\n", lev_str, time_str, __FILE__, __LINE__, ##__VA_ARGS__); \
        }                                                                                                \
    }

#define LOG_DEBUG(format, ...) LOG("DEBUG", DBG_LEVEL, format, DBG_LEVEL);
#define LOG_INFO(format, ...) LOG("INFO", INF_LEVEL, format, INF_LEVEL);
#define LOG_ERROR(format, ...) LOG("ERROR", ERR_LEVEL, format, ERR_LEVEL);
}

#endif