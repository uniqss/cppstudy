#pragma once

// int 就好好int 还const 还& 吃撑了吗
extern "C" __declspec(dllexport) int Add(int a, int b);

extern "C" __declspec(dllexport) int Max(int a, int b);
extern "C" __declspec(dllexport) int Get(int a);
