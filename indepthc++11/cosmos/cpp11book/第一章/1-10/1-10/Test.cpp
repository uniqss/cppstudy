#include <iostream>
using std::cout;
using std::endl;

int main(void) {
    int a = 1.1;
    // int b = {1.1};  // error
    int b = {1024};
    cout << b << endl;

    float fa = 1e40;
    // float fb = {1e40};  // error

    float fc = (unsigned long long)-1;
    // float fd = {(unsigned long long)-1};  // error
    float fe = (unsigned long long)1;
    float ff = {(unsigned long long)1};  // OK

    const int x = 1024;
    const int y = 1;

    char c = x;
    // char d = {x};  // error
    char e = y;
    char f = {y};

    cout << "总结：带 {} 赋值一定要类型完全一致，不要隐式转换。一般隐式转换会报warning，但是如果用 {} "
            "赋值的话会直接报error "
         << endl;

    system("pause");
    return 0;
}