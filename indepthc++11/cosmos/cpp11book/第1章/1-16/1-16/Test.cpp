#include <iostream>
using std::cout;
using std::endl;

void func(void) {
    //...
    cout << __LINE__ << __func__ << endl;
}

struct Foo {
    void operator()(void) {
        //...
        cout << __LINE__ << __func__ << endl;
    }
};

struct Bar {
    using fr_t = void (*)(void);

    static void func(void) {
        //...
        cout << __LINE__ << __func__ << endl;
    }

    // 隐式转换成了静态函数指针 Bar::func
    operator fr_t(void) {
        cout << __LINE__ << __func__ << " implicit conversion." << endl;
        return func;
    }
};

struct A {
    int a_;

    void mem_func(void) {
        //...
        cout << __LINE__ << __func__ << endl;
    }
};

int main(void) {
    void (*func_ptr)(void) = &func;  //函数指针
    func_ptr();

    Foo foo;  //仿函数
    foo();

    Bar bar;  //可被转换为函数指针的类对象
    bar();

    void (A::*mem_func_ptr)(void) = &A::mem_func;  //类成员函数指针
    int A::*mem_obj_ptr = &A::a_;                  //类成员指针

    A aa;
    (aa.*mem_func_ptr)();
    aa.*mem_obj_ptr = 123;

    cout << "函数指针基本用法、仿函数基本用法、可被转换为函数指针的类对象的基本用法、类成员函数指针类成员指针的基本用法"
         << endl;

    system("pause");
    return 0;
}