#include <array>
#include <thread>
#include <iostream>
#include <mutex>

class Counter{ // 封装计数器
public:
    void inc()
    {
        counter_mutex.lock();
        m_count++; // critical section临界区
        counter_mutex.unlock();
    }
    int get(){ // 防止多个线程调用get函数
        int temp;
        counter_mutex.lock();
        temp = m_count;
        counter_mutex.unlock();
        return temp;
    }
private:
    std::mutex counter_mutex;
    int m_count = 0;
};

int main(void)
{
    Counter counter;
    auto increase = [](Counter& counter){for(int i=0; i<1000000; i++) counter.inc();};
    std::thread th1(increase, std::ref(counter));
    std::thread th2(increase, std::ref(counter));
    
    th1.join();
    th2.join();
    
    std::cout << "counter= " << counter.get() << std::endl;
}