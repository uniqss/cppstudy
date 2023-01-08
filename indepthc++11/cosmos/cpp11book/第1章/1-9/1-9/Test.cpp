#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using std::cout;
using std::endl;

class FooVector {
    std::vector<int> content_;

   public:
    // initializer_list 的优点：可以动态数量的参数，声明比较优雅。 缺点：要限定类型
    FooVector(std::initializer_list<int> list) {
        for (auto it = list.begin(); it != list.end(); ++it) {
            content_.push_back(*it);
        }
    }
};

class FooMap {
    std::map<int, int> content_;
    using pair_t = std::map<int, int>::value_type;

   public:
    // 必须限定类型
    FooMap(std::initializer_list<pair_t> list) {
        for (auto it = list.begin(); it != list.end(); ++it) {
            content_.insert(*it);
        }
    }
};

void test(std::initializer_list<int> param) {
    cout << __FILE__ << __func__ << endl;
    for (auto it : param) {
        cout << it << endl;
    }
    cout << endl;
}
// 如果是限定死的类型，推荐用initializer_list，如果不是，可能要完美转发
void pass_initializer_list(std::initializer_list<int> param) {
    test(param);
}

// 多个参数类型的可变模板参数的完美转发
#if 0
void test_rm() {}
template <class T, class... Args>
void test_rm(T&& value, Args&&... args) {
    cout << value << endl;
    test_rm(std::forward<Args>(args)...);
}
template <class... Args>
void pass_rm(Args&&... args) {
    cout << "pass_rm" << endl;
    test_rm(std::forward<Args&&>(args)...);
    cout << endl;
}
#endif
#if 0
// 更炉火纯青的写法
void pass_rm() {
    cout << endl;
}
template <typename T, typename... Args>
void pass_rm(T t, Args&&... args) {
    cout << t << " ";
    pass_rm(std::forward<Args&&>(args)...);
}
#endif
#if 1
// 更取巧的写法，使用c++17 fold
template <typename... Args>
void pass_rm(Args&&... args) {
    ((cout << args << " "), ...);
    cout << endl;
}
#endif

int main(void) {
    FooVector foo1 = {1, 2, 3, 4, 5};
    FooMap foo2 = {{1, 2}, {3, 4}, {5, 6}};
    (1, 2);
    ("hello", 1024, "world");

    pass_initializer_list({1, 2, 3, 4, 5, 6, 7, 8, 9});

    std::string world = "world";
    double d = 123.456;
    bool b = true;
    pass_rm(123, 456, "hello", world, d, b);

    cout << "初始化列表(initializer_list)的使用，完美转发的使用与初始化列表的对比" << endl;

    system("pause");
    return 0;
}