#include<iostream>
#include<stdio.h>
using namespace std;

template <class T, class... Args>
inline void my_printf(T fmt, Args&& ... args){
	printf(fmt, std::forward<Args>(args)...);	
}

int main(){
	my_printf("test1 %d\n", 123);
	my_printf("test1 %d %d\n", 123, 456);
	my_printf("test1 %s\n", "asdf");
	my_printf("test1 %s %s\n", "asdf", "hello");
	my_printf("test1 %d %s\n", 123, "asdf");
	return 0;
}

