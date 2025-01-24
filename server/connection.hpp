#ifndef __CONNECTION_H__
#define __CONNECTION_H__
#include "channel.hpp"

namespace mq
{
    class Connection
    {
    public:
        using ptr = std::shared_ptr<Connection>;
        Connection(const VirtualHost::ptr &host,
                   const ConsumerManager::ptr &cmp,
                   const ProtobufCodecPtr &codec,
                   const muduo::net::TcpConnectionPtr &conn,
                   const threadpool::ptr &pool)
            : _conn(conn),
              _codec(codec),
              _cmp(cmp),
              _host(host),
              _pool(pool),
              _channels(std::make_shared<ChannelManager>())
        {
        }

        void openChannel(const openChannelRequestPtr &req)
        {
            // 1. 判断信道 id 是否重复 , 创建信道
            bool ret = _channels->openChannel(req->rid(), _host, _cmp, _codec, _conn, _pool);
            if (ret == false)
            {
                LOG_DEBUG("创建信道的时候, 信道 id 重复了!");
                return basicResponse(false, req->rid(), req->cid());
            }
            // 2.回复客户端
            return basicResponse(true, req->rid(), req->cid());
        }
        void closeChannel(const closeChannelRequestPtr &req)
        {
            _channels->closeChannel(req->cid());
            return basicResponse(true, req->rid(), req->cid());
        }

        Channel::ptr getChannel(const std::string &cid)
        {
            return _channels->getChannel(cid);
        }

    private:
        void basicResponse(bool ok, const std::string &rid, const std::string &cid)
        {
            basicCommonResponse resp;
            resp.set_rid(rid);
            resp.set_cid(cid);
            resp.set_ok(ok);
            _codec->send(_conn, resp);
        }
        muduo::net::TcpConnectionPtr _conn;
        ProtobufCodecPtr _codec;
        ConsumerManager::ptr _cmp;
        VirtualHost::ptr _host;
        threadpool::ptr _pool;
        ChannelManager::ptr _channels;
    };

    class ConnectionManager
    {
    public:
        using ptr = std::shared_ptr<ConnectionManager>;
        ConnectionManager() {}
        void newConnection(const VirtualHost::ptr &host,
                           const ConsumerManager::ptr &cmp,
                           const ProtobufCodecPtr &codec,
                           const muduo::net::TcpConnectionPtr &conn,
                           const threadpool::ptr &pool)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _conns.find(conn);
            if (it != _conns.end())
                return;
            Connection::ptr self_con = std::make_shared<Connection>(host, cmp, codec, conn, pool);
            _conns.insert(std::make_pair(conn, self_con));
        }
        void delConnection(const muduo::net::TcpConnectionPtr &conn)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _conns.erase(conn);
        }

        Connection::ptr getConnection(const muduo::net::TcpConnectionPtr &conn)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _conns.find(conn);
            if (it == _conns.end())
            {
                return Connection::ptr();
            }
            return it->second;
        }

    private:
        std::mutex _mutex;
        std::unordered_map<muduo::net::TcpConnectionPtr, Connection::ptr> _conns;
    };
}

#endif