#ifndef __CHANNEL_H__
#define __CHANNEL_H__
#include "../common/logger.hpp"
#include "../common/helper.hpp"
#include "../common/msg.pb.h"
#include "../common/proto.pb.h"
#include "../common/threadpool.hpp"
#include "muduo/proto/codec.h"
#include "muduo/proto/dispatcher.h"
#include "consumer.hpp"
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <unordered_map>

namespace mq
{
    using ProtobufCodecPtr = std::shared_ptr<ProtobufCodec>;
    using basicConsumeResponsePtr = std::shared_ptr<basicConsumeResponse>;
    using basicCommonResponsePtr = std::shared_ptr<basicCommonResponse>;
    class Channel
    {
    public:
        using ptr = std::shared_ptr<Channel>;
        Channel(const muduo::net::TcpConnectionPtr &conn, const ProtobufCodecPtr &codec)
            : _cid(UUIDHelper::uuid()), _conn(conn), _codec(codec)
        {
        }
        ~Channel()
        {
            basicCancel();
        }

        std::string cid() const { return _cid; }
        bool declareExchange(const std::string &name,
                             ExchangeType type,
                             bool durable,
                             bool auto_delete,
                             google::protobuf::Map<std::string, std::string> &args)
        {
            // 构造一个声明虚拟机的请求对象.
            std::string rid = UUIDHelper::uuid();
            declareExchangeRequest req;
            req.set_cid(_cid);
            req.set_rid(rid);
            req.set_exchange_name(name);
            req.set_exchange_type(type);
            req.set_durable(durable);
            req.set_auto_delete(auto_delete);
            req.mutable_args()->swap(args);

            // 向服务器发送请求
            _codec->send(_conn, req);

            // 等待服务器的响应
            basicCommonResponsePtr resp = waitResponse(rid);

            return resp->ok();
        }

        void deleteExchange(const std::string &name)
        {
            std::string rid = UUIDHelper::uuid();
            deleteExchangeRequest req;
            req.set_cid(_cid);
            req.set_rid(rid);
            req.set_exchange_name(name);

            _codec->send(_conn, req);
            waitResponse(rid);
            return;
        }

        bool declareQueue(const std::string &name,
                          bool durable,
                          bool exclusive,
                          bool auto_delete,
                          google::protobuf::Map<std::string, std::string> &args)
        {
            std::string rid = UUIDHelper::uuid();
            declareQueueRequest req;
            req.set_cid(_cid);
            req.set_rid(rid);
            req.set_queue_name(name);
            req.set_durable(durable);
            req.set_exclusive(exclusive);
            req.set_auto_delete(auto_delete);
            req.mutable_args()->swap(args);

            _codec->send(_conn, req);
            basicCommonResponsePtr resp = waitResponse(rid);

            return resp->ok();
        }
        void deleteQueue(const std::string &name)
        {
            std::string rid = UUIDHelper::uuid();
            deleteQueueRequest req;
            req.set_cid(_cid);
            req.set_rid(rid);
            req.set_queue_name(name);

            _codec->send(_conn, req);
            waitResponse(rid);
            return;
        }

        bool queueBind(const std::string &exchange_name,
                       const std::string &queue_name,
                       const std::string &binding_key)
        {
            std::string rid = UUIDHelper::uuid();
            queueBindRequest req;
            req.set_cid(_cid);
            req.set_rid(rid);
            req.set_exchange_name(exchange_name);
            req.set_queue_name(queue_name);
            req.set_binding_key(binding_key);

            _codec->send(_conn, req);
            basicCommonResponsePtr resp = waitResponse(rid);
            return resp->ok();
        }

        void queueUnbind(const std::string &exchange_name,
                         const std::string &queue_name)
        {
            std::string rid = UUIDHelper::uuid();
            queueUnBindRequest req;
            req.set_cid(_cid);
            req.set_rid(rid);
            req.set_exchange_name(exchange_name);
            req.set_queue_name(queue_name);

            _codec->send(_conn, req);
            waitResponse(rid);
            return;
        }

        void basicPublish(const std::string &exchange_name,
                          const BasicProperties *bp,
                          const std::string &body)
        {
            std::string rid = UUIDHelper::uuid();
            basicPublishRequest req;
            req.set_cid(_cid);
            req.set_rid(rid);
            req.set_exchange_name(exchange_name);
            req.set_body(body);
            if (bp)
            {
                req.mutable_properties()->CopyFrom(*bp);
            }

            _codec->send(_conn, req);
            waitResponse(rid);
            return;
        }

        void basicAck(const std::string &qname, const std::string &msg_id)
        {
            std::string rid = UUIDHelper::uuid();
            basicAckRequest req;
            req.set_cid(_cid);
            req.set_rid(rid);
            req.set_queue_name(qname);
            req.set_message_id(msg_id);

            _codec->send(_conn, req);
            waitResponse(rid);
            return;
        }

        void basicCancel()
        {
            if (_consumer.get() != nullptr)
            {
                return;
            }
            std::string rid = UUIDHelper::uuid();
            basicCancelRequest req;
            req.set_cid(_cid);
            req.set_rid(rid);
            req.set_consumer_tag(_consumer->tag);
            req.set_queue_name(_consumer->qname);

            _codec->send(_conn, req);
            waitResponse(rid);
            return;
        }

        bool basicConsume(const std::string &ctag, const std::string &qname, bool auto_ack, const ConsumerCallback &cb)
        {
            if (_consumer.get() != nullptr)
            {
                LOG_DEBUG("消费者已经订阅了队列!");
                return false;
            }
            std::string rid = UUIDHelper::uuid();
            basicConsumeRequest req;
            req.set_cid(_cid);
            req.set_rid(rid);
            req.set_consumer_tag(ctag);
            req.set_queue_name(qname);
            req.set_auto_ack(auto_ack);
            req.set_queue_name(qname);
            req.set_consumer_tag(ctag);

            _codec->send(_conn, req);
            basicCommonResponsePtr resp = waitResponse(rid);
            if (!resp->ok())
            {
                LOG_DEBUG("消费者订阅失败!");
                return false;
            }
            _consumer = std::make_shared<Consumer>(ctag, qname, auto_ack, cb);
            return true;
        }

    private:
        // 连接收到基础响应, 向 hash_map 中插入响应
        void putBasicResponse(const basicCommonResponsePtr &resp)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _basic_resp[resp->rid()] = resp;
            _cv.notify_all();
        }

        // 连接收到消费推送后,需要通过信道找到对应消费者对象, 调用消费者对象的回调函数
        void consume(const basicConsumeResponsePtr &resp)
        {
            if (_consumer.get() == nullptr)
            {
                LOG_DEBUG("消息处理时, 未找到订阅者信息!");
                return;
            }
            if (_consumer->tag != resp->consumer_tag())
            {
                LOG_DEBUG("收到的推送消息中的消费者表示与当前信道消费者标识不一致!");
                return;
            }
            _consumer->callback(resp->consumer_tag(), resp->mutable_properties(), resp->body());
        }

        basicCommonResponsePtr waitResponse(const std::string &rid)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _cv.wait(lock, [this, &rid]()
                     { return _basic_resp.find(rid) != _basic_resp.end(); });

            basicCommonResponsePtr ret = _basic_resp[rid];
            _basic_resp.erase(rid);
            return ret;
        }

    private:
        std::string _cid;
        muduo::net::TcpConnectionPtr _conn;
        ProtobufCodecPtr _codec;
        Consumer::ptr _consumer;
        std::mutex _mutex;
        std::condition_variable _cv;
        std::unordered_map<std::string, basicCommonResponsePtr> _basic_resp;
    };

    class ChannelManager
    {
    public:
        using ptr = std::shared_ptr<ChannelManager>;

        ChannelManager() {}
        Channel::ptr create(const muduo::net::TcpConnectionPtr &conn, const ProtobufCodecPtr &codec)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto channel = std::make_shared<Channel>(conn, codec);
            _channels[channel->cid()] = channel;
            return channel;
        }

        void remove(const std::string &cid)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _channels.erase(cid);
        }

        Channel::ptr get(const std::string &cid)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _channels.find(cid);
            if (it == _channels.end())
            {
                return Channel::ptr();
            }
            return it->second;
        }

    private:
        std::mutex _mutex;
        std::unordered_map<std::string, Channel::ptr> _channels;
    };
}

#endif