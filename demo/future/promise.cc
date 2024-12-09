#include <iostream>
#include <thread>
#include <future>

void Add(int num1, int num2, std::promise<int> &prom)
{
    prom.set_value(num1 + num2);
    return;
}
int main()
{
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    std::thread thread(Add, 11, 22, std::ref(prom));
    thread.join();
    int res = fut.get();
    std::cout << res << std::endl;
    return 0;
}