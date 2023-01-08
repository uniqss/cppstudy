#include <iostream>
#include <functional>
using std::cout;
using std::endl;

void output(int x, int y) {
    std::cout << x << " " << y << std::endl;
}

int main(void) {
    std::bind(output, 1, 2)();
    {
        // 这样写更清晰一点，下同
        std::function<void()> func = std::bind(output, 1, 2);
        func();
        cout << endl;
    }
    std::bind(output, std::placeholders::_1, 2)(1);
    {
        std::function<void(int)> func = std::bind(output, std::placeholders::_1, 2);
        func(1);
        cout << endl;
    }
    std::bind(output, 2, std::placeholders::_1)(1);
    {
        std::function<void(int)> func = std::bind(output, 2, std::placeholders::_1);
        func(1);
        cout << endl;
    }

    // std::bind(output, 2, std::placeholders::_2)(1);  //error:调用时没有第二个参数

    std::bind(output, 2, std::placeholders::_2)(1, 2);  //输出 2 2   调用时第一个参数被吞掉了
    {
        std::function<void(int, int)> func = std::bind(output, 1024, std::placeholders::_2);
        // 第一个参数随便传啥，都会被吃掉
        func(1, 2);
        func(2048, 2);
        cout << endl;
    }

    std::bind(output, std::placeholders::_1, std::placeholders::_2)(1, 2);  //输出 1 2
    {
        // 中规中矩
        std::function<void(int, int)> func = std::bind(output, std::placeholders::_1, std::placeholders::_2);
        func(1, 2);
        cout << endl;
    }
    std::bind(output, std::placeholders::_2, std::placeholders::_1)(1, 2);  //输出 2 1
    {
        // 还可以调换 std::placeholders::的顺序来颠倒参数顺序
        std::function<void(int, int)> func = std::bind(output, std::placeholders::_2, std::placeholders::_1);
        func(1, 2);
        cout << endl;
    }
    {
        // 除了能颠倒，还能复用，有点像 fmtlib {1} {1}的用法。  另外这时候第2个参数也是被无视了
        std::function<void(int, int)> func = std::bind(output, std::placeholders::_1, std::placeholders::_1);
        func(1, 2);
        cout << endl;
    }

    cout << "std::bind使用中placeholders的高级用法示例" << endl;

    system("pause");
    return 0;
}