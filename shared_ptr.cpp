#include <iostream>
#include <memory>
using namespace std;

struct Bar {
    int i = 123;
};
struct Foo {
    Bar bar;
};

int main() {
    shared_ptr<Foo> f = make_shared<Foo>();
    cout << f.use_count() << endl; // prints 1
    shared_ptr<Bar> b(f, &(f->bar));
    cout << f.use_count() << endl; // prints 2
    cout << b->i << endl; // prints 123
    return 0;
}

// class Ball {
// public: 
//     Ball() {
//         cout << "A ball appears." << endl;
//     }
//     ~Ball() {
//         cout << "A ball disappears." << endl;
//     }
//     void Bounce() {
//         cout << "A ball jumps." << endl;
//     }
// };

// int main() {
//     shared_ptr<Ball> p = make_shared<Ball>();
//     cout << p.use_count() << endl;
//     shared_ptr<Ball> p2 = p;
//     cout << p.use_count() << " " << p2.use_count() << endl;
//     shared_ptr<Ball> p3 = p;
//     cout << p.use_count() << " " << p2.use_count() << " " << p3.use_count() << endl;
//     p.reset();
//     p2.reset();
//     p3.reset();
//     return 0;
// }