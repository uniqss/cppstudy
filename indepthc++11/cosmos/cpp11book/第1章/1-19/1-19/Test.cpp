#include <iostream>
#include <functional>

void call_when_even(int x, const std::function<void(int)>& f) {
    if (!(x & 1))  // x % 2 == 0
    {
        f(x);
    }
}

void output(int x) {
    std::cout << x << " ";
}

int main(void) {
    for (int i = 0; i < 10; ++i) {
        call_when_even(i, output);
    }

    std::cout << std::endl;

    std::cout << "function "
                 "可以传着到处跑，实在是方便，可以是函数指针，可以是成员函数指针，可以是lambda表达式，可以是可调用对象"
              << std::endl;

    system("pause");
    return 0;
}