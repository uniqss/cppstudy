#include <iostream>
using std::cout;
using std::endl;

void PrintT(int& t) {
    ++t;
    std::cout << "### lvalue t:" << t << std::endl;
}

template <typename T>
void PrintT(T&& t) {
    ++t;
    std::cout << "*** rvalue t:" << t << std::endl;
}

template <typename T>
void TestForward(T&& v) {
    PrintT(v);
    PrintT(std::forward<T>(v));
    PrintT(std::move(v));
    cout << v << endl;
}

void Test() {
    TestForward(1);
    // const是不行的， int& 和 std::forward<int> 都不行
    // const int x = 1;
    int x = 1;
    TestForward(x);
    cout << x << endl;
    TestForward(std::forward<int>(x));
    cout << x << endl;
}

int main(void) {
    Test();

    cout << "右值引用与forward/move使用示例" << endl;

    system("pause");
    return 0;
}
