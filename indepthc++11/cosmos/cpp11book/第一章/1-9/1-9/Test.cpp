#include <iostream>
#include <vector>
#include <map>
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
void test_rm() {
    // cout << "empty func" << endl;
}

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

int main(void) {
    FooVector foo1 = {1, 2, 3, 4, 5};
    FooMap foo2 = {{1, 2}, {3, 4}, {5, 6}};

    pass_initializer_list({1, 2, 3, 4, 5, 6, 7, 8, 9});

    std::string world = "world";
    double d = 123.456;
    bool b = true;
    pass_rm(123, 456, "hello", world, d, b);

    system("pause");
    return 0;
}