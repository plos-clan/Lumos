
# 变量

## 变量声明 `var`

使用类似 C 的形式 `类型 变量名 = 表达式;` 声明并初始化一个变量。<br>
也可以使用类似 C++ 的类型推导 `var 变量名 = 表达式;` 进行声明并初始化。<br>
*注意不是 `auto`*

```lumos
var      a = 初始化表达式;
类型     b = 初始化表达式;
var 类型 c = 初始化表达式;
```

```lumos
var     a = 1; // 这四个等效
int     b = 1;
var int c = 1;
int var d = 1;
```

***允许的优化***

对于一个变量，取到的值可以是：

- 编译时能够确定的值（非 `volatile`）<br>
  *<span style="color:orange">warning</span>: <span style="color:darkcyan">对于编译期可确定的值使用 `lit`</span>*
- 上次修改后缓存的值（非 `volatile`）
- 对应内存地址当前的值

## 不可变变量声明 `val`

使用 `val` 关键字声明常量。方法与 C/C++ 的 `const` 类似。

```lumos
val      常量名 = 值;
val 类型 常量名 = 值;
类型 val 常量名 = 值;
```

```lumos
val     a = 1; // 这三个等效
val int b = 1;
int val c = 1;
```

常量设计上在整个生命周期中不可变，但实际上可以强行修改，但进行修改时可能会因编译器的优化策略导致不可预测的结果。

```lumos
val a = 1;     // 定义常量
println(a);    // 输出 1
*(int*)&a = 2; // 强行修改为 2
println(a);    // 由于编译器的优化策略，可能输出 1 或 2
```

***允许的优化***

对于一个常量，取到的值可以是：

- 编译时能够确定的值（非 `volatile`）<br>
  *<span style="color:orange">warning</span>: <span style="color:darkcyan">对于编译期可确定的值使用 `lit`</span>*
- 作用域内任意位置缓存的值（非 `volatile`）
- 对应内存地址当前的值

## 初始化

所有变量都会被初始化，即使没有给定初始化表达式。

```lumos
int   a; // 这会被初始化为 0
float b; // 这会被初始化为 0.0
void* c; // 这会被初始化为 null
```

变量类型与表达式类型不同时变量初始化被视为显式类型转换。但之后的赋值只允许隐式类型转换。

```lumos
int* a = 0x123456;
```

## 全局不可变变量

你不应该声明一个全局的不可变变量，应该用 `let`(表达式) 或 `lit`(常量表达式) 代替。

见 [表达式](expression.md)。

## 连续的变量声明

可以使用逗号 `,` 连续声明多个变量。

```lumos
int a = 1, b = 2, c = 3;
int* d = &a, e = &b;
// d 和 e 都是指针
```

## `restrict` 限定符

<span style="color:green">注意 `restrict` 不是属性</span>

`restrict` 限定符用于指针，表示指针所指向的内存区域不会被其他指针访问。

```lumos
int* restrict a = malloc(4);
int* restrict b = a; // 这是不可以的
int*          c = a; // 这也是不可以的
```
