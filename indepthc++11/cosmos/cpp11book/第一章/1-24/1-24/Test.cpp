#include <iostream>
#include <vector>
#include <algorithm>
using std::cout;
using std::endl;

class CountEven {
    int& count_;

   public:
    CountEven(int& count) : count_(count) {}

    // 注意这里有两个括号连在一起，第一个是整个的 operator()的一部分，第二个才是参数
    void operator()(int val) {
        if (!(val & 1))  // val % 2 == 0
        {
            ++count_;
        }
    }
};

// 这个示例展示了 std::for_each 对可调用对象的使用方式，会对每个对象都调用可调用对象的operator()
int main(void) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    int even_count = 0;

    std::for_each(v.begin(), v.end(), CountEven(even_count));
    std::cout << "The number of even is " << even_count << std::endl;

    cout << "std::for_each 对可调用对象的使用方式，会对每个对象都调用可调用对象的operator()" << endl;

    system("pause");
    return 0;
}