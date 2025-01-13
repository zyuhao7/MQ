#include "../server/connection.hpp"

int main()
{
    mq::ChannelManager::ptr cmp = std::make_shared<mq::ChannelManager>();
    cmp->openChannel("c1",
                     std::make_shared<mq::VirtualHost>("host1", "./data/host1/message/", "./data/host1/host1.db"),
                     std::make_shared<mq::ConsumerManager>(),
                     mq::ProtobufCodecPtr(),
                     muduo::net::TcpConnectionPtr(),
                     threadpool::ptr());
}