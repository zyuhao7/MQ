#include<iostream>
#include <gtest/gtest.h>

using namespace std;

// 测试 Gtest
int add(int a, int b)
{
    return a + b;
}

TEST(testCase, test1)
{
    EXPECT_EQ(add(2, 3), 5);
}
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// [==========] Running 1 test from 1 test case.
// [----------] Global test environment set-up.
// [----------] 1 test from testCase
// [ RUN      ] testCase.test1
// [       OK ] testCase.test1 (0 ms)
// [----------] 1 test from testCase (0 ms total)

// [----------] Global test environment tear-down
// [==========] 1 test from 1 test case ran. (0 ms total)
// [  PASSED  ] 1 test.