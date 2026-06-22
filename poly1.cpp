// 含有虚函数类型的对象，每个对象初始化的时候，就会附带一个虚函数列表，指向实际类型的虚函数
// vfptr;
#include <iostream>

class Animal {
public:
    int animal;
    void Walk();
};


class Animal1 {
public:
    int animal1;
    virtual void Walk1(); // 虚函数用于实现多态
};

void Animal::Walk() {
    std::cout << "walk" << std::endl;
}

void Animal1::Walk1() {
    std::cout << "walk1" << std::endl;
}

int main() {
    std::cout << sizeof(Animal) << std::endl; // 输出4
    std::cout << sizeof(Animal1) << std::endl; // 输出16
    return 0;
}