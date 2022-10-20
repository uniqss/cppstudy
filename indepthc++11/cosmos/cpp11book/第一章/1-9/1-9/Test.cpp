#include <iostream>
#include <vector>
#include <map>
using std::cout;
using std::endl;

class FooVector {
    std::vector<int> content_;

   public:
    // initializer_list ���ŵ㣺���Զ�̬�����Ĳ����������Ƚ����š� ȱ�㣺Ҫ�޶�����
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
    // �����޶�����
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
// ������޶��������ͣ��Ƽ���initializer_list��������ǣ�����Ҫ����ת��
void pass_initializer_list(std::initializer_list<int> param) {
    test(param);
}

// ����������͵Ŀɱ�ģ�����������ת��
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