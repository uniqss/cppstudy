#include <iostream>
#include <type_traits>
using std::cout;
using std::endl;

int fn(int) {
    return int();
}
typedef int (&fn_ref)(int);
typedef int (*fn_ptr)(int);
struct fn_class {
    int operator()(int i) { return i; }
};

int main(void) {
#if _HAS_CXX17
    typedef std::invoke_result<decltype(fn)&(int)> A;  // int
    typedef std::invoke_result<fn_ref(int)> B;         // int
    typedef std::invoke_result<fn_ptr(int)> C;         // int
    typedef std::invoke_result<fn_class(int)> D;       // int
#else
    typedef std::result_of<decltype(fn)&(int)>::type A;  // int
    typedef std::result_of<fn_ref(int)>::type B;         // int
    typedef std::result_of<fn_ptr(int)>::type C;         // int
    typedef std::result_of<fn_class(int)>::type D;       // int
#endif

    std::cout << std::boolalpha;
    std::cout << "typedefs of int:" << std::endl;

    std::cout << "A: " << std::is_same<int, A>::value << std::endl;
    std::cout << "B: " << std::is_same<int, B>::value << std::endl;
    std::cout << "C: " << std::is_same<int, C>::value << std::endl;
    std::cout << "D: " << std::is_same<int, D>::value << std::endl;
    cout << "#############################" << endl;

    {
        //std::cout << "A: " << std::is_same<int, std::invoke_result<fn(123)>>::value << std::endl;
        std::cout << "A: " << std::is_same<int, A>::value << std::endl;
    }
    cout << "����û�㶮����c++11/14��׼������ok�ģ���������17��20�Ժ�����������ȷд��û�ҵ�����ѧ��17��20�����������о�" << endl;

    system("pause");
    return 0;
}