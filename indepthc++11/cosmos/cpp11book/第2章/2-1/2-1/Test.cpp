#include <iostream>
#include <string>
#include <typeinfo>
#include <memory>
#include <cstdlib>

using std::cout;
using std::endl;

#ifndef _MSC_VER
#include <cxxabi.h>
#endif

template <class T>
std::string type_name() {
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void (*)(void*)> own(
#ifndef __GNUC__
        nullptr,
#else
		abi::__cxa_demangle(typeid(TR).name()), nullptr,
		nullptr, nullptr),
#endif
        std::free);

    std::string r = own != nullptr ? own.get() : typeid(TR).name();

    if (std::is_const<TR>::value) {
        r += " const";
    }
    if (std::is_volatile<TR>::value) {
        r += " volatile";
    }

    if (std::is_lvalue_reference<T>::value) {
        r += "&";
    } else if (std::is_rvalue_reference<T>::value) {
        r += "&&";
    }

    return r;
}

template <typename T>
void MyPrintTypeName(T&& t) {
    std::cout << type_name<T>() << std::endl;
}

template <typename T>
void PrintTypeName(T&& t) {
    std::cout << typeid(T).name() << std::endl;
}

template <class T>
void ComparePrintTypeName(T&& t) {
    cout << " MyPrintTypeName ****************** " << endl;
    MyPrintTypeName(t);
    MyPrintTypeName(std::move(t));
    cout << endl;
    cout << " PrintTypeName by typeid(xxx).name ****************** " << endl;
    PrintTypeName(t);
    PrintTypeName(std::move(t));
    cout << "######################" << endl;
}

int main(void) {
    {
        std::string str = "Test";
        ComparePrintTypeName(str);
    }
    {
        const std::string str = "Test";
        ComparePrintTypeName(str);
    }
    {
        volatile std::string str = "Test";
        ComparePrintTypeName(str);
    }
    {
        const volatile std::string str = "Test";
        ComparePrintTypeName(str);
    }
    {
        int i = 0;
        ComparePrintTypeName(i);
    }
    {
        const int i = 0;
        ComparePrintTypeName(i);
    }
    {
        volatile int i = 0;
        ComparePrintTypeName(i);
    }
    {
        const volatile int i = 0;
        ComparePrintTypeName(i);
    }

    cout << "这个例子展示了标准的 typeid(xx).name() 取不到的 const 和volatile "
            "要怎么取到，以及左右值引用的取法，对代码的调试有一定的帮助"
         << endl;

    system("pause");
    return 0;
}