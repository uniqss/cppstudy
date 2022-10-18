#include <iostream>
using std::cout;
using std::endl;

class MyParam {
   public:
    MyParam(int p) : m_param(p) { cout << "MyParam ctor" << endl; }
    MyParam(const MyParam& mp) {
        cout << "MyParam copy ctor" << endl;
        this->m_param = mp.m_param;
    }
    MyParam& operator=(const MyParam& p) {
        cout << "MyParam assign ctor" << endl;
        if (this == &p) return *this;
        this->m_param = p.m_param;
        return *this;
    }
    MyParam(MyParam&& p) noexcept : m_param(p.m_param) {
        cout << "MyParam move ctor" << endl;
        p.m_param = 0;
    }
    ~MyParam() { cout << "MyParam dtor" << endl; }
    int m_param;
};

std::ostream& operator<<(std::ostream& os, const MyParam& p) {
    os << p.m_param << endl;
    return os;
}

// function参数c++20标准是可以auto的
void func(auto a = 1);  // error:auto不能用于函数参数

// c++20的 auto 在函数参数里的作用已经非常厉害，类似模板参数的推导
void func2(auto param) {
    cout << param << endl;
    cout << typeid(param).name() << endl;
}

// 这里还可以更优化一点，省去一次拷贝构造一次析构
void func3(const auto& param) {
    cout << param << endl;
    cout << typeid(param).name() << endl;
}

// 这样就直接move进来了，不会拷贝构造也不会赋值构造
void func4(auto&& param) {
    cout << param << endl;
    cout << typeid(param).name() << endl;
}

struct Foo {
    // auto var1_ = 0;  // error:auto不能用于非静态成员变量
    static const auto var2_ = 0;
};

template <typename T>
struct Bar {};

int main(void) {
    int arr[10] = {0};
    auto aa = arr;  // OK: aa -> int *
    cout << typeid(aa).name() << endl;
    // auto rr[10] = arr;  // error:auto无法定义数组
    Bar<int> bar;
    // Bar<auto> bb = bar;  // error:auto无法推导出模板参数
    auto bb2 = bar;
    cout << typeid(bb2).name() << endl;

    func2(1234);
    func2("asdf");
    std::string str = "hello world";
    func2(str);

    {
        MyParam mp(123);
        func2(mp);
    }

    cout << "================================" << endl << endl;
    {
        MyParam mp(456);
        func3(mp);
    }

    cout << "================================" << endl << endl;
    {
        MyParam mp(789);
        func4(mp);
    }

    system("pause");
    return 0;
}