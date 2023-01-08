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
    // c++17及以后，这样写
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
    // 
    typedef std::invoke_result<decltype(fn), int>::type A;  // int
    typedef std::invoke_result<fn_ref, int>::type B;        // int
    typedef std::invoke_result<fn_ptr, int>::type C;        // int
    typedef std::invoke_result<fn_class, int>::type D;      // int
#else
    // c++14及以前，这样写
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
        // std::cout << "A: " << std::is_same<int, std::invoke_result<fn(123)>>::value << std::endl;
        std::cout << "A: " << std::is_same<int, A>::value << std::endl;
    }
    cout << "result_of(17及以后要用invoke_result而且用法有细微的差别)的用法，注意result_of能在不调用函数的情况下，拿到它的返回类型"
         << endl;

    system("pause");
    return 0;
}