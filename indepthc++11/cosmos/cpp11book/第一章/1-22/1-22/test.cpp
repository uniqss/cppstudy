#include <iostream>
#include <functional>

class A {
   public:
    int i_ = 0;

    void output(int x, int y) { std::cout << x << " " << y << std::endl; }
};

int main(void) {
    A a;
    // 注意 A::output 并不是一个static的函数，依然可以这样用
    std::function<void(int, int)> fr = std::bind(&A::output, &a, std::placeholders::_1, std::placeholders::_2);
    fr(1, 2);  //输出 1 2

    // vs2022已经不报错了
    // 这个用法用点神奇，可以把类的成员变量也变成一个std::function
    std::function<int&(void)> fr_i = std::bind(&A::i_, &a);  // vs13的bug，绑定成员变量要报错
    fr_i() = 123;

    {
        // 绑定一个局部变量好像是不行的，或者是我的用法不以？？？
        int test = 1024;
        // std::function<int&(void)> func = std::bind(&test, &test);
        // func() = 2024;
    }

    std::cout << a.i_ << std::endl;  //输出 123

    system("pause");
    return 0;
}