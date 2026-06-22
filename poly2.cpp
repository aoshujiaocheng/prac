#include <iostream>

class Basic {
public:
    Basic();
    ~Basic();
private:
    void *buf;
};

Basic::Basic(): buf(std::malloc(16)) {} 

Basic::~Basic() {
    std::free(buf);
    std::cout << "~Basic" << std::endl;
} 

class Child : public Basic {
public:
    Child();
    ~Child();
private:
    void *ch_buf;
};

Child::Child() : Basic(), ch_buf(std::malloc(8)) {}
Child::~Child() {
    std::free(ch_buf);
    std::cout << "~Child" << std::endl;
}


class Child2: public Basic {
public:
    Child2();
    ~Child2();
private:
    void *ch_buf;
};

Child2::Child2() : Basic(), ch_buf(std::malloc(8)) {}
Child2::~Child2() {
    std::free(ch_buf);
    std::cout << "~Child2" << std::endl;
}

Basic *Create(int cmd) {
    if(cmd == 1) return new Child;
    if(cmd == 2) return new Child2;
    return nullptr;
}

void Destroy(Basic *b) {
    delete b;
}

void Demo() {
    // Child ch;
    auto b = Create(1);
    Destroy(b);
}

int main() {
    Demo();
    return 0;
}