#include "connection.hpp"

int main()
{

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
    // 5. 循环向交换机发布消息
    for (int i = 0; i < 10; i++)
    {
        mq::BasicProperties bp;
        bp.set_id(mq::UUIDHelper::uuid());
        bp.set_delivery_mode(mq::DeliveryMode::DURABLE);
        bp.set_routing_key("news.music.pop");

        channel->basicPublish("exchange1", &bp, "hello word - " + std::to_string(i));
    }

    mq::BasicProperties bp;
    bp.set_id(mq::UUIDHelper::uuid());
    bp.set_delivery_mode(mq::DeliveryMode::DURABLE);
    bp.set_routing_key("news.music.sport");

    channel->basicPublish("exchange1", &bp, "hello sport !");

    bp.set_routing_key("news.cartoon");
    channel->basicPublish("exchange1", &bp, "hello cartoon!");

    // 6. 关闭信道
    conn->closeChannel(channel);
    return 0;
}