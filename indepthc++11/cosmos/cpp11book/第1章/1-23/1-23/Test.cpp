#include <iostream>

// 这都是lambda的一些基本用法，看看就行了，错误也不修了，就是要展示编译错误的

class A {
    int i_ = 0;

    void func(int x, int y) {
        auto x1 = [] { return i_; };  // error,没有捕获外部变量
        auto x2 = [=] { return i_ + x + y; };
        auto x3 = [&] { return i_ + x + y; };
        auto x4 = [this] { return i_; };
        auto x5 = [this] { return i_ + x + y; };  // error,没有捕获x和y
        auto x6 = [this, x, y] { return i_ + x + y; };
        auto x7 = [this] { return i_++; };
    }
};

int main(void) {
    {
        int a = 0;
        int b = 1;
        auto f1 = [] { return a; };  // error,没有捕获外部变量
        auto f2 = [&] { return a++; };
        auto f3 = [=] { return a; };
        auto f4 = [=] { return a++; };    // error,a是以复制方式捕获的，无法修改
        auto f5 = [a] { return a + b; };  // error,没有捕获变量b
        auto f6 = [a, &b] { return a + (b++); };
        auto f7 = [=, &b] { return a + (b++); };
    }

    system("pause");
    return 0;
}