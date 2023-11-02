# 第一章 c++ 新规范

## 1 标识符规范

使用一些通用的命名规范可以很大程度提高代码可读性，让协同开发变得容易~~（没人会喜欢读OIer、ACMer的代码吧）~~。下面列出一些常见规范：

### 命名规则

- 标识符由字母、数字、下划线组成
- 第一个字符不能是数字

### 命名约定

- 遵循一致命名风格和约定，如驼峰命名法（camelCase）、下划线命名法（underscore_case）
- 文件的命名方式通常采用下划线命名法
- 在命名类、结构体、枚举、函数、命名空间时每个单词的首字母要大写，如：Analyzer
- 类成员变量后接下划线，如：table_name_ ，结构体不用
- 枚举内部成员用全大写+下划线方式命名，如：OUT_OF_MEMORY

## 2 注释风格

西皮皮有多种注释风格任君选择，这里选择 Doxygen 简要介绍。嵌入式中常用的 HAL 库就是使用这种风格进行注释的。

### 文件开头

```cpp
/**
 * @file header.h
 * @brief Brief file introduction.
 *
 * Detailed file introduction.
 *
 * @author Name
 * @date day month year
 * @see Related link.
 */
```

`@see` 中主要记录一些额外参考信息，比如相关链接等；其他不用过多介绍。

### 函数注释

```cpp
/* GLOBAL FUNCTIONS */
/**
 * @brief Example showing how to document a function with Doxygen.

 * @param [in] param1 Description of the first parameter of the function.
 * @param [out] param2 The second one, which follows @p param1, and represents output.
 *
 * @return Describe what the function returns.
 * @retval XXX_OK if successful.
 *
 * @see <http://website/>
 */
int doxygen_theFirstFunction(int param1, int param2);
```

嫌麻烦只用标 `@param` 就好，不需要 `[in][out]`；`retval`是 return value 的意思

## 3 inline 函数

### 古早用法

> `inline`关键字指示编译器用函数定义中的代码替换函数调用的每个实例。
>
> 使用内联函数可以是程序更快，因为它们消除了与函数调用关联的开销。

这一时期的具体用法：

- 内联函数最适用于小函数使用，例如访问私有的数据成员

### 摩登一些

在cpp的发展历程中，`inline`被赋予了更加重要的功能：避免重复定义。

```cpp
/* foo.h */
inline int foo(int x) {
    static int factor = 1;
    return x * (factor++);
}

/* bar1.cc */
#include "foo.h"
int bar1() {
    return foo(1);
}

/* bar2.cc */
#include "foo.h"
int bar2() {
    return foo(2);
}

/* main.cc */
int bar1(), bar2();
int main() {
    return bar1() + bar2();
}

```

这种用法与使用 `#ifndef`头文件保护的功能类似。不过，`#ifndef`只能实现单文件保护，即单一文件中`#include`多个头文件可以规避重复定义；而示例中使用`inline`则可以实现多文件保护。

但是，用`inline`定义的函数如果包含多种实现，编译器并不会报错，而是在链接过程中可能根据源文件的编译顺序而决定使用哪一个实现。

### 还能更摩登吗

c++ 17 之后，`inline`可以用于修饰变量了。它的具体功能从古早用法中的代码替换转变成了：

> 允许在多个编译单元对同一个变量进行定义，并且在链接时至保留其中的一份作为该符号的定义。

这被广泛运用在全局变量、类中静态数据成员的初始化中：

```cpp
/* foo.h */
struct Foo {
    inline static int foo = 1;
};
/* bar1.cc */
#include "foo.h"
int Bar1() { return Foo::foo + 1; }
/* bar2.cc */
#include "foo.h"
int Bar2() { return Foo::foo + 2; }
/* main.cc */
int Bar1(), Bar2();
int main() {
    return Bar1() + Bar2();
}
```

当然，在这一过程中必须保证统一个`inline`变量的定义都相同。

> `inline`还有一些其它用法，如用`inline`修饰命名空间，在此不多做介绍；同时，c与cpp中`inline`作用也有所不同。

## 4 一些零碎知识

虽然一般开发都有IDE帮忙提示，但为了考试故在此记录一下：

### 函数重载规则

- 函数名称必须相同
- 参数列表不同（个数不同、类型不同、参数排列顺序不同）
- 函数返回类型可以相同也可以不相同
- **仅仅返回类型不同不能实现重载**

### 默认参数

- 有默认值的形参必须放在其它形参之后

### 引用

- 引用必须在创建时初始化，指针可以随时初始化
- 引用初始化后不能再更改作为其他变量的引用，指针可以随时指向其他变量
- 不存在空引用；不能建立引用的数组
- 可以建立指针变量的引用，如：

```cpp
#include <memory>

void foo(std::unique_ptr<int>& a){}

std::unique_ptr<int> a = std::make_unique<int>(3);
foo(a); // 减少一次copy
```

