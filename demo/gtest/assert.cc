#include <iostream>
#include <gtest/gtest.h>

TEST(test, greater_than)
{
    int age = 20;
    ASSERT_GT(age, 18);
    std::cout << "OK! " << std::endl;
}

TEST(test, less_than)
{
    int age = 20;
    ASSERT_LT(age, 18);
    std::cout << "OK! " << std::endl;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    return 0;
}