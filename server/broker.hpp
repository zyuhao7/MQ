#ifndef __BROKE_HPP__
#define __BROKE_HPP__
#include "muduo/proto/codec.h"
#include "muduo/proto/dispatcher.h"
// #include "muduo/base/Logging.h"
#include "muduo/base/Mutex.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"

#include "connection.hpp"
#include "consumer.hpp"
#include "queue.hpp"
#include "host.hpp"
#include "../common/threadpool.hpp"
#include "../common/msg.pb.h"
#include "../common/proto.pb.h"
#include "../common/logger.hpp"

namespace mq
{
#define DBFILE "/meta.db"
#define HOSTNAME "myVirutalHost"
    class Server
    {
    public:
        typedef std::shared_ptr<google::protobuf::Message> MessagePtr;
        Server(int port, const std::string &basedir)
            : _server(&_baseloop, muduo::net::InetAddress("0.0.0.0", port), "Server", muduo::net::TcpServer::kReusePort),
              _dispatcher(std::bind(&Server::onUnknownMessage, this,
                                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)),
              _codec(std::make_shared<ProtobufCodec>(std::bind(&ProtobufDispatcher::onProtobufMessage, &_dispatcher,
                                                               std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))),
              _virtual_host(std::make_shared<VirtualHost>(HOSTNAME, basedir, basedir + DBFILE)),
              _consumer_manager(std::make_shared<ConsumerManager>()),
              _connection_manager(std::make_shared<ConnectionManager>()),
              _threadpool(std::make_shared<threadpool>())
        {
            // 针对历史消息中的所有队列, 初始化队列的消费者管理结构
            QueueMap qm = _virtual_host->getAllQueues();
            for (auto &q : qm)
            {
                _consumer_manager->initQueueConsumer(q.first);
            }

            // 注册业务请求处理函数
            _dispatcher.registerMessageCallback<mq::openChannelRequest>(std::bind(&Server::onOpenChannel, this,
                                                                                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _dispatcher.registerMessageCallback<mq::closeChannelRequest>(std::bind(&Server::onCloseChannel, this,
                                                                                   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _dispatcher.registerMessageCallback<mq::declareExchangeRequest>(std::bind(&Server::onDeclareExchange, this,
                                                                                      std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _dispatcher.registerMessageCallback<mq::deleteExchangeRequest>(std::bind(&Server::onDeleteExchange, this,
                                                                                     std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _dispatcher.registerMessageCallback<mq::declareQueueRequest>(std::bind(&Server::onDeclareQueue, this,
                                                                                   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _dispatcher.registerMessageCallback<mq::deleteQueueRequest>(std::bind(&Server::onDeleteQueue, this,
                                                                                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _dispatcher.registerMessageCallback<mq::queueBindRequest>(std::bind(&Server::onQueueBind, this,
                                                                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _dispatcher.registerMessageCallback<mq::queueUnBindRequest>(std::bind(&Server::onQueueUnBind, this,
                                                                                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _dispatcher.registerMessageCallback<mq::basicPublishRequest>(std::bind(&Server::onBasicPublish, this,
                                                                                   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _dispatcher.registerMessageCallback<mq::basicAckRequest>(std::bind(&Server::onBasicAck, this,
                                                                               std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _dispatcher.registerMessageCallback<mq::basicConsumeRequest>(std::bind(&Server::onBasicConsume, this,
                                                                                   std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _dispatcher.registerMessageCallback<mq::basicCancelRequest>(std::bind(&Server::onBasicCancel, this,
                                                                                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            _server.setMessageCallback(std::bind(&ProtobufCodec::onMessage, _codec.get(),
                                                 std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            _server.setConnectionCallback(std::bind(&Server::onConnection, this, std::placeholders::_1));
        }
        void start()
        {
            _server.start();
            _baseloop.loop();
        }

    private:
        // 打开信道
        void onOpenChannel(const muduo::net::TcpConnectionPtr &conn, const openChannelRequestPtr &message, muduo::Timestamp)
        {
            Connection::ptr mconn = _connection_manager->getConnection(conn);
            if (mconn.get() == nullptr)
            {
                LOG_DEBUG("打开信道时, 没有找到对应的 Connection 对象!");
                conn->shutdown();
                return;
            }
            return mconn->openChannel(message);
        }

        // 关闭信道
        void onCloseChannel(const muduo::net::TcpConnectionPtr &conn, const closeChannelRequestPtr &message, muduo::Timestamp)
        {
            Connection::ptr mconn = _connection_manager->getConnection(conn);
            if (mconn.get() == nullptr)
            {
                LOG_DEBUG("打开信道时, 没有找到对应的 Connection 对象!");
                conn->shutdown();
                return;
            }
            return mconn->closeChannel(message);
        }

        // 声明交换机
        void onDeclareExchange(const muduo::net::TcpConnectionPtr &conn, const declareExchangeRequestPtr &message, muduo::Timestamp)
        {
            Connection::ptr mconn = _connection_manager->getConnection(conn);
            if (mconn.get() == nullptr)
            {
                LOG_DEBUG("声明交换机时, 没有找到对应的 Connection 对象!");
                conn->shutdown();
                return;
            }
            Channel::ptr mchannel = mconn->getChannel(message->cid());
            if (mchannel.get() == nullptr)
            {
                LOG_DEBUG("声明交换机时, 没有找到对应的 Channel 对象!");
                return;
            }
            return mchannel->declareExchange(message);
        }

        // 删除交换机
        void onDeleteExchange(const muduo::net::TcpConnectionPtr &conn, const deleteExchangeRequestPtr &message, muduo::Timestamp)
        {
            Connection::ptr mconn = _connection_manager->getConnection(conn);
            if (mconn.get() == nullptr)
            {
                LOG_DEBUG("删除交换机, 没有找到对应的 Connection 对象!");
                conn->shutdown();
                return;
            }
            Channel::ptr mchannel = mconn->getChannel(message->cid());
            if (mchannel.get() == nullptr)
            {
                LOG_DEBUG("删除交换机, 没有找到对应的 Channel 对象!");
                return;
            }
            return mchannel->deleteExchange(message);
        }

        // 声明队列
        void onDeclareQueue(const muduo::net::TcpConnectionPtr &conn, const declareQueueRequestPtr &message, muduo::Timestamp)
        {
            Connection::ptr mconn = _connection_manager->getConnection(conn);
            if (mconn.get() == nullptr)
            {
                LOG_DEBUG("声明队列时, 没有找到对应的 Connection 对象!");
                conn->shutdown();
                return;
            }
            Channel::ptr mchannel = mconn->getChannel(message->cid());
            if (mchannel.get() == nullptr)
            {
                LOG_DEBUG("声明队列时, 没有找到对应的 Channel 对象!");
                return;
            }
            return mchannel->declareQueue(message);
        }

        // 删除队列
        void onDeleteQueue(const muduo::net::TcpConnectionPtr &conn, const deleteQueueRequestPtr &message, muduo::Timestamp)
        {
            Connection::ptr mconn = _connection_manager->getConnection(conn);
            if (mconn.get() == nullptr)
            {
                LOG_DEBUG("删除队列时, 没有找到对应的 Connection 对象!");
                conn->shutdown();
                return;
            }
            Channel::ptr mchannel = mconn->getChannel(message->cid());
            if (mchannel.get() == nullptr)
            {
                LOG_DEBUG("删除队列时, 没有找到对应的 Channel 对象!");
                return;
            }
            return mchannel->deleteQueue(message);
        }

        // 队列绑定
        void onQueueBind(const muduo::net::TcpConnectionPtr &conn, const queueBindRequestPtr &message, muduo::Timestamp)
        {
            Connection::ptr mconn = _connection_manager->getConnection(conn);
            if (mconn.get() == nullptr)
            {
                LOG_DEBUG("队列绑定时, 没有找到对应的 Connection 对象!");
                conn->shutdown();
                return;
            }
            Channel::ptr mchannel = mconn->getChannel(message->cid());
            if (mchannel.get() == nullptr)
            {
                LOG_DEBUG("队列绑定时, 没有找到对应的 Channel 对象!");
                return;
            }
            return mchannel->queueBind(message);
        }

        // 队列解绑
        void onQueueUnBind(const muduo::net::TcpConnectionPtr &conn, const queueUnBindRequestPtr &message, muduo::Timestamp)
        {
            Connection::ptr mconn = _connection_manager->getConnection(conn);
            if (mconn.get() == nullptr)
            {
                LOG_DEBUG("队列解绑时, 没有找到对应的 Connection 对象!");
                conn->shutdown();
                return;
            }
            Channel::ptr mchannel = mconn->getChannel(message->cid());
            if (mchannel.get() == nullptr)
            {
                LOG_DEBUG("队列解绑时, 没有找到对应的 Channel 对象!");
                return;
            }
            return mchannel->queueUnbind(message);
        }

        // 消息发布
        void onBasicPublish(const muduo::net::TcpConnectionPtr &conn, const basicPublishRequestPtr &message, muduo::Timestamp)
        {
            Connection::ptr mconn = _connection_manager->getConnection(conn);
            if (mconn.get() == nullptr)
            {
                LOG_DEBUG("发布消息时, 没有找到对应的 Connection 对象!");
                conn->shutdown();
                return;
            }
            Channel::ptr mchannel = mconn->getChannel(message->cid());
            if (mchannel.get() == nullptr)
            {
                LOG_DEBUG("发布消息时, 没有找到对应的 Channel 对象!");
                return;
            }
            return mchannel->basicPublish(message);
        }

        // 消息确认
        void onBasicAck(const muduo::net::TcpConnectionPtr &conn, const basicAckRequestPtr &message, muduo::Timestamp)
        {
            Connection::ptr mconn = _connection_manager->getConnection(conn);
            if (mconn.get() == nullptr)
            {
                LOG_DEBUG("消息确认时, 没有找到对应的 Connection 对象!");
                conn->shutdown();
                return;
            }
            Channel::ptr mchannel = mconn->getChannel(message->cid());
            if (mchannel.get() == nullptr)
            {
                LOG_DEBUG("消息确认时, 没有找到对应的 Channel 对象!");
                return;
            }
            return mchannel->basicAck(message);
        }

        // 队列消息订阅
        void onBasicConsume(const muduo::net::TcpConnectionPtr &conn, const basicConsumeRequestPtr &message, muduo::Timestamp)
        {
            Connection::ptr mconn = _connection_manager->getConnection(conn);
            if (mconn.get() == nullptr)
            {
                LOG_DEBUG("队列消息订阅时, 没有找到对应的 Connection 对象!");
                conn->shutdown();
                return;
            }
            Channel::ptr mchannel = mconn->getChannel(message->cid());
            if (mchannel.get() == nullptr)
            {
                LOG_DEBUG("队列消息订阅时, 没有找到对应的 Channel 对象!");
                return;
            }
            return mchannel->basicConsume(message);
        }
        // 队列消息取消订阅
        void onBasicCancel(const muduo::net::TcpConnectionPtr &conn, const basicCancelRequestPtr &message, muduo::Timestamp)
        {
            Connection::ptr mconn = _connection_manager->getConnection(conn);
            if (mconn.get() == nullptr)
            {
                LOG_DEBUG("队列消息取消订阅时, 没有找到对应的 Connection 对象!");
                conn->shutdown();
                return;
            }
            Channel::ptr mchannel = mconn->getChannel(message->cid());
            if (mchannel.get() == nullptr)
            {
                LOG_DEBUG("队列消息取消订阅时, 没有找到对应的 Channel 对象!");
                return;
            }
            return mchannel->basicCancel(message);
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
                _connection_manager->newConnection(_virtual_host, _consumer_manager, _codec, conn, _threadpool);
            }
            else
            {
                _connection_manager->delConnection(conn);
            }
        }

    private:
        muduo::net::EventLoop _baseloop;
        muduo::net::TcpServer _server;  // 服务器对象
        ProtobufDispatcher _dispatcher; // 请求分发器对象 -- 要向其中注册请求处理函数
        ProtobufCodecPtr _codec;        // protobuf协议处理器 -- 针对收到的请求数据进行protobuf协议处理
        VirtualHost::ptr _virtual_host;
        ConsumerManager::ptr _consumer_manager;
        ConnectionManager::ptr _connection_manager;
        threadpool::ptr _threadpool;
    };
}
#endif