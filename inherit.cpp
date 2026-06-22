#include <iostream>

class Animal {
public:
    int member;
    void f();
    using type = int;
};

void Animal::f() {
    std::cout << "f" << std::endl;
}

class Dog : public Animal {
    // 1.成员变量 2.成员函数 3.类型定义
};

void Demo() {
    Dog d;
    d.member = 5;
    d.f();
    Dog::type a = 0;
    std::cout << std::is_same_v<Dog::type, int> << std::endl;

}

int main(int argc, const char *argv[]) {
    Demo();
    return 0;
}