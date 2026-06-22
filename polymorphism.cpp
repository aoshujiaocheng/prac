#include <iostream>
#include <typeinfo>

class Animal {
public:
    virtual void Walk(); // 虚函数用于实现多态
    void Walk1();
};

void Animal::Walk() {
    std::cout << "default" << std::endl;
}

void Animal::Walk1() {
    std::cout << "default" << std::endl;
}

class Dog : public Animal {
    // 1.成员变量 2.成员函数 3.类型定义
public: 
    void Walk() override; // 重写虚函数
    void Walk1();
};

// override
void Dog::Walk() {
    std::cout << "Dog" << std::endl;
}

// name hiding
void Dog::Walk1() {
    std::cout << "Dog" << std::endl;
}

class Cat : public Animal {
    // 1.成员变量 2.成员函数 3.类型定义
    void Walk() override;
    void Walk1();
};

void Cat::Walk() {
    std::cout << "Cat" << std::endl;
}

void Cat::Walk1() {
    std::cout << "Cat" << std::endl;
}

// 虚函数看实际对象类型
// 非虚函数只看引用/指针声明类型（这里形参是Animal&）,永远调用父类Animal::Walk1()，无视子类同名函数
void LetItWalk(Animal &animal) {
    std::cout << typeid(animal).name() << ": ";
    animal.Walk();
    std::cout << typeid(animal).name() << ": ";
    animal.Walk1();
}

void Demo() {
    Dog d;
    Cat c;
    LetItWalk(d);
    LetItWalk(c);
}

int main(int argc, const char *argv[]) {
    Demo();
    return 0;
}