#ifndef __M_WORK_H__
#define __M_WORK_H__
#include "muduo/net/EventLoopThread.h"
#include "../common/logger.hpp"
#include "../common/helper.hpp"
#include "../common/threadpool.hpp"

namespace mq
{
    class AsyncWorker
    {
    public:
        using ptr = std::shared_ptr<AsyncWorker>;

        muduo::net::EventLoopThread loopthread;
        threadpool _pool;
    };
}
#endif