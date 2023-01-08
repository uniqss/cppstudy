#include <iostream>
using std::cout;
using std::endl;

// 传统写法确实比较恶心，一坨一坨的，而且要支持的参数越多，越多坨
template <typename T>
T* Instance() {
    return new T();
}

template <typename T, typename T0>
T* Instance(T0 arg0) {
    return new T(arg0);
}

template <typename T, typename T0, typename T1>
T* Instance(T0 arg0, T1 arg1) {
    return new T(arg0, arg1);
}

template <typename T, typename T0, typename T1, typename T2>
T* Instance(T0 arg0, T1 arg1, T2 arg2) {
    return new T(arg0, arg1, arg2);
}

template <typename T, typename T0, typename T1, typename T2, typename T3>
T* Instance(T0 arg0, T1 arg1, T2 arg2, T3 arg3) {
    return new T(arg0, arg1, arg2, arg3);
}

template <typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
T* Instance(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
    return new T(arg0, arg1, arg2, arg3, arg4);
}

struct A {
    A(int) {}
};

struct B {
    B(int, double) {}
};

struct C {
    C(){}
};

// 简约写法
//template <typename T>
//T* MyInstance() {
//    return new T();
//}
template <typename T, typename... Args>
T* MyInstance(Args&&... args) {
    return new T(std::forward<Args&&>(args)...);
}

template<class T>
class TTT {
    template<typename T1>
    void con() {

    }
};

int main(void) {
    A* pa = Instance<A>(1);
    B* pb = Instance<B>(1, 2);
    C* pc = MyInstance<C>();

    A* mya = MyInstance<A>(1);
    B* myb = MyInstance<B>(1, 2);

    cout << "类的构造器传统多模板参数写法与完美转发写法的对比" << endl;

    delete pa;
    delete pb;
    delete mya;
    delete myb;

    system("pause");
    return 0;
}