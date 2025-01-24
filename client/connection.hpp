#ifndef __CONNECTION_H__
#define __CONNECTION_H__
#include "muduo/proto/dispatcher.h"
#include "muduo/proto/codec.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Mutex.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpClient.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/base/CountDownLatch.h"
#include "channel.hpp"
#include "work.hpp"

namespace mq
{
    class Connection
    {
    public:
        typedef std::shared_ptr<google::protobuf::Message> MessagePtr;

        Connection(const std::string &sip, int sport, const AsyncWorker::ptr &worker)
            : _latch(1),
              _client(worker->loopthread.startLoop(), muduo::net::InetAddress(sip, sport), "Client"),
              _dispatcher(std::bind(&Connection::onUnknownMessage, this, std::placeholders::_1,
                                    std::placeholders::_2, std::placeholders::_3)),
              _codec(std::bind(&ProtobufDispatcher::onProtobufMessage, &_dispatcher,
                               std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
        {

            _dispatcher.registerMessageCallback<basicCommonResponse>(std::bind(&Connection::basicResponse, this,
                                                                               std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _dispatcher.registerMessageCallback<basicConsumeResponse>(std::bind(&Connection::consumeResponse, this,
                                                                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _client.setMessageCallback(std::bind(&ProtobufCodec::onMessage, &_codec,
                                                 std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            _client.setConnectionCallback(std::bind(&Connection::onConnection, this, std::placeholders::_1));
        }
        void connect()
        {
            _client.connect();
            _latch.wait(); // 阻塞等待，直到连接建立成功
        }
        Channel::ptr openChannel();
        void closeChannel(const Channel::ptr &channel);

    private:
        void basicResponse(const muduo::net::TcpConnectionPtr &conn, const basicCommonResponsePtr &message, muduo::Timestamp)
        {
        }
        void consumeResponse(const muduo::net::TcpConnectionPtr &conn, const basicConsumeResponsePtr &message, muduo::Timestamp)
        {
        }

        void onUnknownMessage(const muduo::net::TcpConnectionPtr &conn, const MessagePtr &message, muduo::Timestamp)
        {
            LOG_INFO("onUnknownMessage: %s", message->GetTypeName().c_str());
            conn->shutdown();
        }
        void onConnection(const muduo::net::TcpConnectionPtr &conn)
        {
            if (conn->connected())
            {
                _latch.countDown(); // 唤醒主线程中的阻塞
                _conn = conn;
            }
            else
            {
                // 连接关闭时的操作
                _conn.reset();
            }
        }

    private:
        muduo::CountDownLatch _latch;            // 实现同步的
        muduo::net::EventLoopThread _loopthread; // 异步循环处理线程
        muduo::net::TcpConnectionPtr _conn;      // 客户端对应的连接
        muduo::net::TcpClient _client;           // 客户端
        ProtobufDispatcher _dispatcher;          // 请求分发器
        ProtobufCodec _codec;                    // 协议处理器
        AsyncWorker::ptr _worker;                // 异步工作线程
        ChannelManager::ptr _channel_manager;    // 信道管理器
    };
}

#endif