#include <iostream>
#include <vector>

template <class ContainerT>
class Foo {
#if 0
    typename ContainerT::iterator it_;  //���Ͷ������������
                                        //������ const ContainerT ��ͨ��������ʹ������it_���壺
                                        // decltype(std::declval<ContainerT>().begin()) it_;
#else
    decltype(std::declval<ContainerT>().begin()) it_;
#endif
   public:
    void func(ContainerT& container) {
        it_ = container.begin();
    }

    //...
};

int main(void) {
#if 1
    typedef const std::vector<int> container_t;  //������const���ԣ�����ᱨһ��Ѵ�����Ϣ
#else
    typedef std::vector<int> container_t;  //������const���ԣ�����ᱨһ��Ѵ�����Ϣ
#endif
    container_t arr;

    Foo<container_t> foo;
    foo.func(arr);

    system("pause");
    return 0;
}