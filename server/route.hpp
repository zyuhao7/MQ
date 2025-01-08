#ifndef __ROUTE_H__
#define __ROUTE_H__
#include <iostream>
#include "../common/msg.pb.h"
#include "../common/logger.hpp"
#include "../common/helper.hpp"
namespace mq
{
    class Router
    {
    public:
        static bool isLegalRoutingKey(const std::string &routing_key)
        {
            // 只需要判断是否有非法字符即可
            for (auto &ch : routing_key)
            {
                if (ch >= 'a' && ch <= 'z' ||
                    ch >= 'A' && ch <= 'Z' ||
                    ch >= '0' && ch <= '9' ||
                    ch == '.' || ch == '_')
                    continue;
                return false;
            }
            return true;
        }
        static bool isLegalBindingKey(const std::string &binding_key)
        {
            // 只需要判断是否有非法字符
            for (auto &ch : binding_key)
            {
                if (ch >= 'a' && ch <= 'z' ||
                    ch >= 'A' && ch <= 'Z' ||
                    ch >= '0' && ch <= '9' ||
                    ch == '.' || ch == '_' ||
                    ch == '*' || ch == '#')
                    continue;
                return false;
            }
            // * 和 # 必须独立存在
            std::vector<std::string> sub_keys;
            StrHelper::split(binding_key, ".", sub_keys);

            for (auto &key : sub_keys)
            {
                // 一个单词有多个字符, 且包含 * 或 #, 则返回 false
                if (key.size() > 1 &&
                    (key.find("*") != std::string::npos ||
                     key.find("#") != std::string::npos))
                    return false;
            }
            // * 和 # 不能连续存在
            for (int i = 1; i < sub_keys.size(); i++)
            {
                if (sub_keys[i] == "*" && sub_keys[i - 1] == "#" ||
                    sub_keys[i] == "#" && sub_keys[i - 1] == "*" ||
                    sub_keys[i] == "#" && sub_keys[i - 1] == "#")
                    return false;
            }
            return true;
        }
        static bool route(ExchangeType type, const std::string &routing_key, const std::string &binding_key)
        {
            if (type == ExchangeType::DIRECT)
                return routing_key == binding_key;
            else if (type == ExchangeType::FANOUT)
                return true;
            // 主题交换
            // 1. 将 binding_key 与 routing_key 进行字符串分割, 分割成多个单词
            std::vector<std::string> routing_keys, binding_keys;
            int n_rkey = StrHelper::split(routing_key, ".", routing_keys);
            int n_bkey = StrHelper::split(binding_key, ".", binding_keys);

            // 2.定义标记数组, 初始化 [0][0]位置为 true, 其余位置为 false
            std::vector<std::vector<bool>> dp(n_bkey + 1, std::vector<bool>(n_rkey + 1, false));
            dp[0][0] = true;

            // 3.如果 binding_key 以 # 开始, 则将 # 对应行的第 0 列置为 true
            for (int i = 1; i <= binding_keys.size(); i++)
            {
                if (binding_keys[i - 1] == "#")
                {
                    dp[i][0] = true;
                    continue;
                }
                break;
            }
            // 4. 使用动态规划算法, 从第 1 行开始, 从第 1 列开始, 如果 binding_key 的单词与 routing_key 的单词相等, 则将其置为 true
            for (int i = 1; i <= n_bkey; i++)
            {
                for (int j = 1; j <= n_rkey; j++)
                {
                    // 如果当前 bkey是个单词或者*, 表示单词匹配成功, 从左上方继承.
                    if (binding_keys[i - 1] == routing_keys[j - 1] || binding_keys[i - 1] == "*")
                    {
                        dp[i][j] = dp[i - 1][j - 1];
                    } 
                    // 如果当前 bkey 是个#, 表示匹配成功, 从左上、左边、上方继承.
                    else if (binding_keys[i - 1] == "#")
                    {
                        dp[i][j] = dp[i - 1][j] | dp[i][j - 1] | dp[i - 1][j - 1];
                    }
                }
            }
            return dp[n_bkey][n_rkey];
        }
    };

}

#endif