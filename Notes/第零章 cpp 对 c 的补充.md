## 第零章 cpp 对 c 的补充

### 1.1 命名空间

命名空间是一个声明性区域，为其内部元素（类、函数、变量等）提供一个范围。

这对于完善代码逻辑来说十分有用，例如：将某个类和某个外部函数组合在一起

```cpp
namespace Factory{
    // 创建一个 worker 对象
    class Worker{
        public:
        	int ID;
        	std::string Name;
        	// Clock是Worker的内部方法
        	void Clock(){}
    }
    // OpenGate并不是Worker内部方法，但是隶属于Factory
    void OpenGate(){}
}
```

命名空间中的元素可以通过以下方法进行访问：

 ```cpp
 Factory::Worker worker;
 Factory::OpenGate();
 worker.Clock();
 ```

通过使用 `using` 指令，可以将命名空间中的元素引入当前作用域范围：

```cpp
using Factory::Worker;

Worker worker;
Factory::OpenGate();
worker.Clock();
```

也可以将命名空间中的所有元素引入当前范围：

```cpp
using namespace Factory;

Worker worker;
OpenGate();
worker.Clock();
```

 `using` 指令可以放置在 .cpp 文件的头部或 类和函数 的定义内，但是要避免将其放在 .h 头文件中，避免引发与命名有关的问题。

另外，命名空间中的函数声明在外被定义需要包含命名空间：

```cpp
namespace V{
    void f();
}

void V::f(){}
```

其它命名空间相关特性可见：[命名空间 (C++) | Microsoft Learn](https://learn.microsoft.com/zh-cn/cpp/cpp/namespaces-cpp?view=msvc-170)

### 1.2 const 变量

事实上应该把 `const` 和 `static` 关键字放在一起进行对比，具体内容可以自行查找阅读。

至于 `constexpr` ，笔者也不懂，所以也不加叙述了。

本文这里只讲 `const`  有关部分内容。

#### 1.2.1 const 关键字

原来在写 c 代码时，我们通常使用 `#define` 来定义常量，而 cpp 中则添加了 `const` 关键字以供我们使用。

用 `const` 定义变量时必须在定义的同时进行赋值：

```cpp
const double PI = 3.14;
const double e; // 必须指定变量值，否则报错
```

下面对几个易混淆概念进行解释：

```cpp
const int Max = 100;

const int* a = new int;
// 等同于 int const* a = new int;
*a = 50; // 报错，指向常变量的指针，即不能改变 *a 指向地址中元素的值
a = (int*)&Max;

int* const b = new int;
*b = 50;
b = (int*)&Max; // 报错，常指针，可以改变指向地址中保存的值，但不能改变指针指向的地址

const int* const c = new int;
*c = 50; // 报错
c = (int*)&Max; // 报错，指向常变量的常指针，无论是地址或是地址中保存的值都不能被修改
```

虽然但是，实际开发过程中应该不会写出这样绕人的代码……

现在也不会到处使用这种野指针了（因为太容易让人犯迷糊导致内存泄漏），这在后文讲引用的时候还会提到。

#### 1.2.2 const 成员函数

声明中带 `const`  关键字的成员函数表明该函数是一个只读函数，不能对成员对象做任何修改。

但是带 `mutable` 关键字的成员除外。

```cpp
class Program{
public:
    int a = 1;
    mutable int b = 2;
    
    int fun() const{
        a = 2; // 报错，不能修改成员对象
        b = 5; // 可执行，带mutable关键字的成员对象可在const方法中被修改
        return a;
    }
};
```

### 1.3 extern 关键字

在编写大型项目时，我们在一个 project 里往往会有多个  .cpp / .h 文件，如果全局变量很多的话难免会发生命名冲突问题。

> 在了解extern之前首先要知道C++中得单定义规则。所谓的单定义规则（One Definition Rule,ODR）是指变量只能有一次定义。为了满足这种需求，c++提供了两种变量声明。一种是定义声明（defining declaration）简称定义，它给变量分配内存空间；另外一种是引用声明（referencing declaration）简称为声明，它不给变量分配空间，因为它引用已有变量。
>
> 在全局变量中，引用声明使用extern关键字且不进行初始化，否则声明为定义，导致分配空间。

```cpp
// file01.cpp
int cats = 10;
int dogs = 20; 
```

```cpp
// file02.cpp
extern int cats;
extern int dogs;

int main() {
    std::cout << cats << std::endl;
    dogs = 15;
    std::cout << dogs << std::endl;
}
```

输出如下：

```
10
15
```

如果 `file02.cpp` 中没加 `extern` 关键字，则会发生报错，因为存在命名冲突问题。

另外，被 `extern` 声明的变量必须在其它地方被定义。

关于 `extern` 还有 `extern "C"` 这种特殊用法，具体功能可见：[关于 C++ 中的 extern "C" - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/123269132)

### 1.4 函数重载

函数重载，可以理解为用相同名字函数针对不同变量来实现不同功能。

```cpp
int foo(int x, int y){
    return x+y;
}
int foo(int x, int y, int z){
    return x+y+z;
}
foo(1, 2); // 调用第一个函数定义
foo(1, 2, 3); // 调用第二个函数定义
```

常见的重载规则：

| 函数声明元素 | 是否用于重载 |
| ------------ | ------------ |
| 函数返回类型 | 否           |
| 自变量的数量 | 是           |
| 自变量的类型 | 是           |

### 1.5 函数模板

利用函数模板可以极大增加函数复用率：

```cpp
template<typename Lhs, typename Rhs>
auto Add2(const Lhs& lhs, const Rhs& rhs)
{
    return lhs + rhs;
}

auto a = Add2(3.13, 2.895); // a is a double
auto b = Add2(string{ "Hello" }, string{ " World" }); // b is a std::string
```

### 1.6 引用

与指针类似，引用也将存储位于内存中其它位置的对象的地址，但是，引用无法像指针一样更改引用对象或被设置成 `null` 。

#### 1.6.1 左值引用（&）

只要能取得地址，就是左值。

```cpp
class Person{
    std::string Name;
    int Age;
}

int main(){
    Person Bill;
    Person& myFriend = Bill;
    
    Bill.Name = "Bill";
    myFriend.Age = 20;
    
    std::cout << myFriend.Name << "is" << Bill.Age << std::endl;
    // output: Bill is 20
}
```

左值引用可以分为两种：**非 `const` 左值引用**和 **`const` 左值引用**

非 `const` 左值引用只能绑定左值，而 `const` 左值引用既能绑定左值也能绑定右值

```cpp
int a = 1;
int& lref_a = a;
lref_a ++; // 通过非 const 左值引用可以修改其值
const int& lref_const_a = a;
// lref_const_a++; // error, const左值引用不能修改其值

const int& lref_const_rvalue = 999;  // const 左值引用可以直接绑定右值 999 
cout << "lref_const_rvalue = " << lref_const_rvalue;
```

这样设计的原因是什么？

我们考虑要实现一个  `print` 方法，并且使用引用传递防止额外拷贝，如果使用：

```cpp
void print(int& a);
```

那么在使用的时候可能需要这样：

```cpp
int a = 1;
print(a)
```

但事实上，我们为图方便可能想要这样写：

```cpp
print(1);
```

在这里 1 是右值，为了无论左值还是右值函数都能正常接受参数，故引入了 `const` 左值：

```cpp
void print(const int& a);
```

#### 1.6.2 右值引用（&&）

右值引用只能绑定在右值上：

```cpp
int b = 2;
// int&& rref_b = b; // error,右值引用只能绑定到右值上，b是一个左值
int&& rref_2 = 2; // ok
cout << "rref_2=" << rref_2 << endl;  // 输出 2
rref_2++;
cout << "rref_2=" << rref_2 << endl;  // 输出 3
```

#### 1.6.3 左值向右值的类型转换——move

```cpp
int b = 2;
// int&& rref_b = b; // error,右值引用只能绑定到右值上，b是一个左值
int&& rref_b = std::move(b); // ok, std::move(b) 是一个右值，可以用右值引用绑定
```

无论是左值引用还是右值引用在传参时都有非常大用处，具体可见：[ c++为什么要搞个引用岀来，特别是右值引用，感觉破坏了语法的简洁和条理，拷贝一个指针不是很好吗？ - 知乎 (zhihu.com)](https://www.zhihu.com/question/363686723/answer/2590214399)

### 1.7 范围

cpp 中存在六种范围：全局范围、命名空间范围、局部范围、类范围、语句范围、函数范围。

有一种特殊方法(`::`)可以用来访问全局范围名称：

```cpp
#include <iostream>

int i = 7;   // i has global scope, outside all blocks
using namespace std;

int main( int argc, char *argv[] ) {
   int i = 5;   // i has block scope, hides i at global scope
   cout << "Block-scoped i has the value: " << i << "\n";
   cout << "Global-scoped i has the value: " << ::i << "\n";
}
```

```output
Block-scoped i has the value: 5
Global-scoped i has the value: 7
```



