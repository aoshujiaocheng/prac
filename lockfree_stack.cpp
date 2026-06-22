#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

template<typename T>
class LockFreeStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    
    std::atomic<Node*> head{nullptr};
    
public:
    void push(const T& value) {
        Node* new_node = new Node(value);
        new_node->next = head.load();
        
        // CAS 循环：直到成功将新节点设为新的 head
        while (!head.compare_exchange_weak(new_node->next, new_node)) {
            // 如果失败，new_node->next 会被自动更新为当前的 head
            // 继续尝试
        }
    }
    
    bool pop(T& result) {
        Node* old_head = head.load();
        
        while (old_head != nullptr && 
               !head.compare_exchange_weak(old_head, old_head->next)) {
            // CAS 失败时，old_head 被更新为最新的 head 值
            // 继续循环
        }
        
        if (old_head == nullptr) {
            return false;  // 栈为空
        }
        
        result = old_head->data;
        delete old_head;
        return true;
    }
    
    ~LockFreeStack() {
        T tmp;
        while (pop(tmp));
    }
};

// 测试代码
int main() {
    LockFreeStack<int> stack;
    std::vector<std::thread> threads;
    
    // 多线程 push
    for (int i = 0; i < 100; ++i) {
        threads.emplace_back([&stack, i]() {
            stack.push(i);
        });
    }
    
    // 多线程 pop
    std::atomic<int> sum{0};
    for (int i = 0; i < 50; ++i) {
        threads.emplace_back([&stack, &sum]() {
            int value;
            if (stack.pop(value)) {
                sum += value;
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Sum of popped values: " << sum.load() << std::endl;
    return 0;
}