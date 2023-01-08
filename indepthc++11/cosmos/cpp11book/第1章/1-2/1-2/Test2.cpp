#include <iostream>

using std::cout;
using std::endl;

class Foo {
   public:
    static int get(void) { return 0; }
};

class Bar {
   public:
    static const char* get(void) { return "0"; }
};

template <class A>
void func(void) {
    // 模板函数中使用 auto 获取类的静态成员函数的返回类型
    auto val = A::get();

    //...
    cout << "val:" << val << " type:" << typeid(val).name() << endl;
}

int main(void) {
    func<Foo>();
    func<Bar>();

    cout << "模板函数中使用 auto 获取类的静态成员函数的返回类型 的用法展示" << endl;

    system("pause");
    return 0;
}