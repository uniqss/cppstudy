#include <iostream>
using std::cout;
using std::endl;

// 这个3怎么看起来有点多余？？？是错觉吗
int i_arr[3] = {1, 2, 3};  //普通数组

// 好像不是错觉
int i_arr2[] = {1, 2, 3};  //普通数组

struct A {
    int x;
    struct B {
        int i;
        int j;
    } b;
} a = {1, {2, 3}};  // POD类型

int i = 0;

class Foo {
   public:
    Foo(int) {}
} foo = 123;

int j(0);

Foo bar(123);

int main(void) {
    cout << "类对象直接赋值的用法展示（纯POD可嵌套或者带参构造函数）" << endl;
    system("pause");
    return 0;
}