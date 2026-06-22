#include <iostream>
// 禁用拷贝、赋值需要用到
#include <mutex>
using namespace std;

// 单例类
class Singleton {
private:
    // 1.构造函数私有化： 禁止外部new创建对象
    Singleton() {
        cout << "单例对象创建成功!" << endl;
    }
    // 2.禁用拷贝构造+赋值运算符
    Singleton(const Singleton&) =delete;
    Singleton& operator=(const Singleton&) =delete;

public:
    // 3.全局访问点：获取唯一实例
    static Singleton& getInstance(){
        // c++11静态局部变量：线程安全，只会初始化一次
        static Singleton instance;
        return instance;
    }
};

int main(){
    // 获取单例对象
    Singleton& s1 = Singleton::getInstance();
    Singleton& s2 = Singleton::getInstance();
    
    // 验证
    cout << "s1地址: " << &s1 << endl;
    cout << "s2地址: " << &s2 << endl;
    
    return 0;
}