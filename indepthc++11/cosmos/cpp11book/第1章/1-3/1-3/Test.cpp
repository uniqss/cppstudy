#include <iostream>
using std::cout;
using std::endl;

class Foo {
   public:
    static const int Number = 0;
    int x;
};

int main(void) {
    int n = 0;
    volatile const int &x = n;

    decltype(n) a = n;  // a -> int
    cout << typeid(a).name() << endl;
    decltype(x) b = n;  // b -> const volatile int &
    cout << typeid(b).name() << endl;

    decltype(Foo::Number) c = 0;  // c -> const int
    cout << typeid(c).name() << endl;

    Foo foo;
    decltype(foo.x) d = 1024;  // d -> int
    cout << typeid(d).name() << endl;

    // 为什么打印出来一个二个都是int？？？
    // 拿c试一下,,,明明是const int，为什么打印出来是int???
    // c = 2048; // 会报错
    // 查了一下，c++的 type_info 是会吃掉 const volatile 的，只有带指针时才会显现出来 参见下面的链接：
    // https://stackoverflow.com/questions/10007596/type-info-doesnt-account-for-cv-qualifiers-is-this-right

    cout << "decltype的用法展示 typeid不能获取const volatile的问题展示(第二章中有获取cv的方式)" << endl;

    system("pause");
    return 0;
}