// MyDll.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "MyDll.h"

#include <iostream>
using std::cout;
using std::endl;

extern "C" __declspec(dllexport) int Add(int a, int b) {
    cout << "Add a:" << a << " b:" << b << endl;
    return a + b;
}

extern "C" __declspec(dllexport) int Max(int a, int b) {
    cout << "Max a:" << a << " b:" << b << endl;
    return a >= b ? a : b;
}

extern "C" __declspec(dllexport) int Get(int a) {
    cout << "Get a:" << a << endl;
    int ret = a * a + 1;
    return ret;
}
