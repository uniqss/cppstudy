#include "MyDLL.h"

#include <iostream>
using std::cout;
using std::endl;

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

int TestDll(std::string& errString) {
    int ret = -1;
    typedef int (*pMax)(int a, int b);
    typedef int (*pGet)(int a);

    HINSTANCE hDLL = nullptr;

    do {
        hDLL = LoadLibrary("MyDll.dll");
        if (hDLL == nullptr) {
            errString = "LoadLibrary failed";
            break;
        }

        pMax Max = (pMax)GetProcAddress(hDLL, "Max");
        if (Max == nullptr) {
            errString = "GetProcAddress Max failed";
            break;
        }

        ret = Max(5, 8);
        cout << "Max ret:" << ret << endl;

        pGet Get = (pGet)GetProcAddress(hDLL, "Get");
        if (Get == nullptr) {
            errString = "GetProcAddress Get failed";
            break;
        }
        ret = Get(5);
        cout << "Get ret:" << ret << endl;

        ret = 0;
    } while (false);

    if (hDLL != nullptr) FreeLibrary(hDLL);

    return ret;
}

int main(void) {
    int ret = 0;
    std::string errString;
    ret = TestDll(errString);
    if (ret != 0) {
        cout << "TestDll errString:" << errString << endl;
    }

    cout << "windows 直接访问DLL里面函数的使用方式" << endl;

    system("pause");
    return 0;
}