#include <iostream>
using std::cout;
using std::endl;

template <typename T>
void Print(T t) {
    std::cout << t << std::endl;
}

template <typename T1, typename T2>
void Print(T1 t1, T2 t2) {
    std::cout << t1 << t2 << std::endl;
}

template <typename T1, typename T2, typename T3>
void Print(T1 t1, T2 t2, T3 t3) {
    std::cout << t1 << t2 << t3 << std::endl;
}

template <typename T1, typename T2, typename T3, typename T4>
void Print(T1 t1, T2 t2, T3 t3, T4 t4) {
    std::cout << t1 << t2 << t3 << t4 << std::endl;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void Print(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) {
    std::cout << t1 << t2 << t3 << t4 << t5 << std::endl;
}

// 简约写法，支持无限参数
void MyPrint() {
    cout << endl;
}
template <typename T, typename... Args>
void MyPrint(T t, Args&&... args) {
    cout << t << " ";
    MyPrint(std::forward<Args&&>(args)...);
}


int main(void) {
    Print(1);
    Print(1, 2);
    Print(1, 2, 3);
    Print(1, 2, 3, 4);
    Print(1, 2, 3, 4, 5);

    {
        MyPrint(1);
        MyPrint(1, "asdf");
        MyPrint(1, 2);
        MyPrint(1, 2, "asdf", "jkl", 345, "hello");
    }

    cout << "这里展示了没有c++11的可变模板参数之前的有多少个参数就要重载多少份的冗余写法。补充了简约写法" << endl;

    system("pause");
    return 0;
}