#include <iostream>
#include <thread>
#include <future>

int Add(int num1, int num2)
{
    std::cout << "Add Func " << std::endl;
    return num1 + num2;
}

int main()
{
    //     async(launch __policy, _Fn&& __fn, _Args&&... __args)
    std::future<int> result = std::async(std::launch::deferred, Add, 11, 22);
    std::cout << result.get() << std::endl;
    return 0;
}