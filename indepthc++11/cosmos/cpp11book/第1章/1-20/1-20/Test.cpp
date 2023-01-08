#include <iostream>
#include <functional>
using std::cout;
using std::endl;

void call_when_even(int x, const std::function<void(int)>& f) {
    if (!(x & 1))  // x % 2 == 0
    {
        f(x);
    }
}

void output(int x) {
    std::cout << x << " ";
}

void output_add_2(int x) {
    std::cout << x + 2 << " ";
}

int main(void) {
    {
        // auto fr = std::bind(output, std::placeholders::_1);
        // 这样写可能更直观一点，std::bind可以把函数指针lambda表达式可调用对象之类的转换成std::function
        // 另外注意这里的 std::placeholders::_1
        std::function<void(int)> fr = std::bind(output, std::placeholders::_1);
        for (int i = 0; i < 10; ++i) {
            call_when_even(i, fr);
        }

        std::cout << std::endl;
    }
    {
        auto fr = std::bind(output_add_2, std::placeholders::_1);

        for (int i = 0; i < 10; ++i) {
            call_when_even(i, fr);
        }

        std::cout << std::endl;
    }

    cout << "std::bind的基本用法，std::placeholders的用法" << endl;

    system("pause");
    return 0;
}