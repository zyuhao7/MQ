#include "../server/queue.hpp"
#include <gtest/gtest.h>
mq::MsgQueueManager::ptr mqp;
class QueueTest : public testing::Environment
{
public:
    virtual void SetUp() override
    {
        mqp = std::make_shared<mq::MsgQueueManager>("./data/meta.db");
    }

    virtual void TearDown() override
    {
        // mqp->clear();
        std::cout << "最后清理!\n";
    }
};

TEST(queue_test, insert_test)
{
    std::unordered_map<std::string, std::string> map{{"k1", "v1"}};
    // std::unordered_map<std::string, std::string> map;

    mqp->declareQueue("queue1", true, false, false, map);
    mqp->declareQueue("queue2", true, false, false, map);
    mqp->declareQueue("queue3", true, false, false, map);
    mqp->declareQueue("queue4", true, false, false, map);
    ASSERT_EQ(mqp->size(), 4);
}

TEST(queue_test, select_test)
{
    ASSERT_EQ(mqp->exists("queue1"), true);
    ASSERT_EQ(mqp->exists("queue2"), true);
    ASSERT_EQ(mqp->exists("queue3"), true);
    ASSERT_EQ(mqp->exists("queue4"), true);

    mq::MsgQueue::ptr exp = mqp->selectQueue("queue1");
    ASSERT_NE(exp.get(), nullptr);
    ASSERT_EQ(exp->name, "queue1");
    ASSERT_EQ(exp->durable, true);
    ASSERT_EQ(exp->auto_delete, false);
    ASSERT_EQ(exp->getArgs(), std::string("k1=v1&"));
}

TEST(queue_test, remove_test)
{
    mqp->deleteQueue("queue2");
    mq::MsgQueue::ptr exp = mqp->selectQueue("queue2");
    ASSERT_EQ(exp.get(), nullptr);
    ASSERT_EQ(mqp->exists("queue2"), false);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment(new QueueTest);
    return RUN_ALL_TESTS();
}