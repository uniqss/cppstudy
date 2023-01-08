# Indepth C++ 11

### 第1章 \cosmos\cpp11book\第1章\

- 1-1 auto 的基本使用 移动语义临时变量的优化展示
- 1-2 模板函数中使用 auto 获取类的静态成员函数的返回类型 的用法展示
- 1-3 decltype 的用法展示 typeid 不能获取 const volatile 的问题展示(第二章中有获取 cv 的方式)
- 1-4 decltype 获取函数返回值类型的用法展示，并不需要实际调用函数，不需要函数实现部分，只需要定义
- 1-5 decltype 配合 declval 获取容器 Iterator 类型的用法展示,declval 能获取包括 const 的信息
- 1-7 类对象直接赋值的用法展示（纯 POD 可嵌套或者带参构造函数）
- 1-8 拷贝构造的使用，根据类构造函数参数列表进行初始化的使用
- 1-9 初始化列表(initializer_list)的使用，完美转发的使用与初始化列表的对比
- 1-10 总结：带 {} 赋值一定要类型完全一致，不要隐式转换。一般隐式转换会报 warning，但是如果用 {} 赋值的话会直接报 error
- 1-11~1-15 Range 的实现及使用
- 1-16 函数指针基本用法、仿函数基本用法、可被转换为函数指针的类对象的基本用法、类成员函数指针类成员指针的基本用法
- 1-17 std::function 的基本用法（函数指针、类静态成员函数指针、仿函数对象）
- 1-19 function 可以传着到处跑，实在是方便，可以是函数指针，可以是成员函数指针，可以是 lambda 表达式，可以是可调用对象
- 1-20 std::bind 的基本用法，std::placeholders 的用法
- 1-21 std::bind 使用中 placeholders 的高级用法示例
- 1-22 std::bind 在类的非静态成员函数上的用法示例、类的成员上的用法示例
- 1-23 这都是 lambda 的一些基本用法，看看就行了，错误也不修了，就是要展示编译错误的
- 1-24 std::for_each 对可调用对象的使用方式，会对每个对象都调用可调用对象的 operator()

### 第2章 \cosmos\cpp11book\第2章\

- 2-1 这个例子展示了标准的 typeid(xx).name() 取不到的 const 和 volatile 要怎么取到，以及左右值引用的取法，对代码的调试有一定的帮助. type_name 的完美实现
- 2-2 这个示例展示了在 c++11 出来以前，没有 std::move 时，c++被诟病的临时对象的性能问题
- 2-3 与 2-2 对比，看出 c++11 使用 std::move 移动语义解决了临时对象的性能问题
- 2-4 右值引用与 forward/move 使用示例
- 2-5 这里展示了 map.emplace 和 vector.emplace_back 的强大之处 直接原地 construct 连 move 都省了
- 2-6 这里展示了 unordered_map 的基本用法，包括自定义类作为 Key 的用法

### 第3章 \cosmos\cpp11book\第3章\

- 3-1 remove_reference 和 add_lvalue_reference 在模板类中的用法
- 3-2 result_of(17及以后要用invoke_result而且用法有细微的差别)的用法，注意result_of能在不调用函数的情况下，拿到它的返回类型
- 3-3 这里展示了没有 c++11 的可变模板参数之前的有多少个参数就要重载多少份的冗余写法。补充了简约写法
- 3-4 类的构造器传统多模板参数写法与完美转发写法的对比
- 3-5 这里简单的封装了一个 boost 里面的 optional
- 3-7~3-8 这里简单封装了一个 lazy，可以懒加载，定义出来的时候只存了一个函数指针和一堆完美转发的参数，适用于特别慢的资源的加载，注意这里并没有提供线程安全机制
- 3-9 windows 直接访问 DLL 里面函数的使用方式
- 3-10

### 第4章 \cosmos\cpp11book\第4章\

### 第5章 \cosmos\cpp11book\第5章\

### 第6章 \cosmos\cpp11book\第6章\
