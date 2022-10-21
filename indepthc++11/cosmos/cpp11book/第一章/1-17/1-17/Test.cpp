#include <iostream>
#include <functional>
using std::cout;
using std::endl;

void func(void) {
    std::cout << __FUNCTION__ << std::endl;
}

class Foo {
   public:
    static int foo_func(int a) {
        std::cout << __FUNCTION__ << "(" << a << ") ->: ";
        return a;
    }
};

class Bar {
   public:
    int operator()(int a) {
        std::cout << __FUNCTION__ << "(" << a << ") ->: ";
        return a;
    }
};

int main(void) {
    std::function<void(void)> fr1 = func;
    fr1();

    //绑定一个类的静态成员函数
    std::function<int(int)> fr2 = Foo::foo_func;
    std::cout << fr2(123) << std::endl;

    Bar bar;
    fr2 = bar;
    std::cout << fr2(456) << std::endl;

    cout << "std::function的基本用法（函数指针、类静态成员函数指针、仿函数对象）" << endl;

    system("pause");
    return 0;
}