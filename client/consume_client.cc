#include "connection.hpp"
void cb(mq::Channel::ptr &channel, std::string consume_tag, const mq::BasicProperties *bp, const std::string &body)
{
    std::cout << consume_tag << "消费了消息 : " << body << std::endl;
    channel->basicAck(bp->id());
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " queue1" << std::endl;
        return -1;
    }
    // 1. 实例化异步工作线程对象
    mq::AsyncWorker::ptr awp = std::make_shared<mq::AsyncWorker>();
    // 2. 实例化连接对象
    mq::Connection::ptr conn = std::make_shared<mq::Connection>("127.0.0.1", 8085, awp);
    // 3.通过连接创建信道
    mq::Channel::ptr channel = conn->openChannel();
    // 4.通过信道提供的服务完成所需
    //  1. 声明交换机Exchange1, 交换机类型为广播模式
    google::protobuf::Map<std::string, std::string> tmp_map;
    channel->declareExchange("exchange1", mq::ExchangeType::TOPIC,
                             true, false, tmp_map);
    //  2. 声明队列queue1
    channel->declareQueue("queue1", true, false, false, tmp_map);
    //  3. 声明队列queue2
    channel->declareQueue("queue2", true, false, false, tmp_map);
    //  4. 将队列queue1绑定到交换机Exchange1,且 binding key 为queue1
    channel->queueBind("exchange1", "queue1", "queue1");
    //  5. 将队列queue2绑定到交换机Exchange1,且 binding key 为 news.music.#
    channel->queueBind("exchange1", "queue2", "news.music.#");

    auto functor = std::bind(cb, channel, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    channel->basicConsume("consumer1", argv[1], false, functor);
    while (1)
        std::this_thread::sleep_for(std::chrono::seconds(3));
    // 6. 关闭信道
    conn->closeChannel(channel);
    return 0;
}