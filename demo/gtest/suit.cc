#include <iostream>
#include <gtest/gtest.h>
#include <unordered_map>
class MyTest : public testing::Test
{
public:
    static void SetUpTestCase()
    {
        std::cout << "所有单元测试前执行, 初始化总环境\n";
    }
    void SetUp() override
    {
        std::cout << "单元测试初始化\n";

        _mymap.insert(std::make_pair("hello", "Nih"));
        _mymap.insert(std::make_pair("bye", "Zaij"));
    }

    void TearDown() override
    {
        std::cout << "单元测试清理\n";
        _mymap.clear();
    }
    static void TearDownCase()
    {
        std::cout << "所有单元测试前执行, 清理总环境\n";
    }

public:
    std::unordered_map<std::string, std::string> _mymap;
};

TEST_F(MyTest, insert_test)
{
    _mymap.insert(std::make_pair("housailei", "猴赛雷"));
    ASSERT_EQ(_mymap.size(), 3);
}

TEST_F(MyTest, size_test)
{
    ASSERT_EQ(_mymap.size(), 2);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    RUN_ALL_TESTS();
    return 0;
}