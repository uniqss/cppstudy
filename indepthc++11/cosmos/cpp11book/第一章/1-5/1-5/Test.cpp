#include <iostream>
#include <vector>
using std::cout;
using std::endl;

template <class ContainerT>
class Foo {
#if 0
    typename ContainerT::iterator it_;  //类型定义可能有问题
                                        //若想在 const ContainerT 下通过编译请使用下面it_定义：
                                        // decltype(std::declval<ContainerT>().begin()) it_;
#else
    // 这里只是示例，实际项目中不要在类中存取Iterator，因为c++的容器的Iterator都是会失效的
    decltype(std::declval<ContainerT>().begin()) it_;
#endif
   public:
    void func(ContainerT& container) {
        // 实际上应该在这里用，是可以的，不要存储就行，出作用域即消失。另外这些代码都是非线程安全的
        it_ = container.begin();
    }

    //...
};

int main(void) {
#if 1
    typedef const std::vector<int> container_t;  //这里有const属性，编译会报一大堆错误信息
#else
    typedef std::vector<int> container_t;  //这里有const属性，编译会报一大堆错误信息
#endif
    container_t arr;

    Foo<container_t> foo;
    foo.func(arr);

    cout << "decltype配合declval获取容器Iterator类型的用法展示,declval能获取包括const的信息" << endl;

    system("pause");
    return 0;
}
