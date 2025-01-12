#ifndef __CHANNEL_H__
#define __CHANNEL_H__
#include "../common/logger.hpp"
#include "../common/helper.hpp"
#include "../common/msg.pb.h"
#include "../common/proto.pb.h"
#include "../common/threadpool.hpp"
#include "muduo/proto/codec.h"
#include "muduo/proto/dispatcher.h"
#include "muduo/net/TcpConnection.h"
#include "consumer.hpp"
#include "host.hpp"

namespace mq
{
    using ProtobufCodecPtr = std::shared_ptr<ProtobufCodec>;
    using openChannelRequestPtr = std::shared_ptr<openChannelRequest>;
    using closeChannelRequestPtr = std::shared_ptr<closeChannelRequest>;
    using declareExchangeRequestPtr = std::shared_ptr<declareExchangeRequest>;
    using deleteExchangeRequestPtr = std::shared_ptr<deleteExchangeRequest>;
    using declareQueueRequestPtr = std::shared_ptr<declareQueueRequest>;
    using deleteQueueRequestPtr = std::shared_ptr<deleteQueueRequest>;
    using queueBindRequestPtr = std::shared_ptr<queueBindRequest>;
    using queueUnBindRequestPtr = std::shared_ptr<queueUnBindRequest>;
    using basicPublishRequestPtr = std::shared_ptr<basicPublishRequest>;
    using basicAckRequestPtr = std::shared_ptr<basicAckRequest>;
    using basicConsumeRequestPtr = std::shared_ptr<basicConsumeRequest>;
    using basicCancelRequestPtr = std::shared_ptr<basicCancelRequest>;

    class Channel
    {
    public:
        Channel(const std::string &id,
                const VirtualHost::ptr &host,
                ConsumerManager::ptr &cmp,
                const ProtobufCodecPtr &codec,
                const muduo::net::TcpConnectionPtr &conn,
                const threadpool::ptr &pool)
            : _cid(id),
              _conn(conn),
              _codec(codec),
              _pool(pool)
        {

        }

        ~Channel()
        {
            if (_consumer.get() != nullptr)
            {
                _cmp->remove(_consumer->tag, _consumer->qname);
            }
        }
        // 交换机的声明与删除
        void declareExchange(const declareExchangeRequestPtr &req)
        {
            _host->declareExchange(req->exchange_name(), req->exchange_type(),
            )
        }
        void deleteExchange(const deleteExchangeRequestPtr &req);

        // 队列的声明与删除
        void declareQueue(const declareQueueRequestPtr &req);
        void deleteQueue(const deleteQueueRequestPtr &req);

        // 队列的绑定与解除绑定
        void queueBind(const queueBindRequestPtr &req);
        void queueUnbind(const queueUnBindRequestPtr &req);

        // 消息的发布
        void basicPublish(const basicPublishRequestPtr &req);

        // 消息的确认
        void basicAck(const basicAckRequestPtr &req);

        // 订阅队列消息
        void basicConsume(const basicConsumeRequestPtr &req);

        // 取消订阅
        void basicCancel(const basicCancelRequestPtr &req);

    private:
        std::string _cid;
        Consumer::ptr _consumer;
        muduo::net::TcpConnectionPtr _conn;
        ProtobufCodecPtr _codec;
        ConsumerManager::ptr _cmp;
        VirtualHost::ptr _host;
        threadpool::ptr _pool;
    };
}

#endif