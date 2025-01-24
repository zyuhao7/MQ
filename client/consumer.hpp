#ifndef __CONSUMER_H__
#define __CONSUMER_H__
#include "../common/logger.hpp"
#include "../common/helper.hpp"
#include "../common/msg.pb.h"
#include <unordered_map>
#include <iostream>
#include <mutex>
#include <memory>
#include <vector>
#include <functional>
namespace mq
{
    using ConsumerCallback = std::function<void(const std::string tag, const BasicProperties *bp, const std::string body)>;
    struct Consumer
    {
        using ptr = std::shared_ptr<Consumer>;
        std::string tag;   // 消费者标识
        std::string qname; // 消费者订阅队列名称
        bool auto_ack;     // 自动确认标志
        ConsumerCallback callback;

        Consumer() {}
        Consumer(const std::string &ctag, const std::string &que_name, bool ack_flag, const ConsumerCallback &cb)
            : tag(ctag),
              qname(que_name),
              auto_ack(ack_flag),
              callback(std::move(cb))
        {
        }
        ~Consumer() {}
    };
}
#endif