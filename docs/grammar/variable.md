
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

Lumos 允许的初始化方式有：

- 赋值初始化 `int my_var = 1;`
- 构造函数初始化 `int my_var(1);`<br>
  <span style="color:green">对于基本数据类型有伪构造函数</span>
- 结构体元素赋值初始化 `MyStructure my_var = {1, 2, .third = 3};`
- 数组元素赋值初始化 `int my_var[] = {1, 2, [2] = 3};`
- 默认初始化 `int my_var;`<br>
  <span style="color:green">默认初始化会将基本数据类型变量初始化为二进制 0，对于其它数据类型则调用默认构造函数</span>
  <span style="color:green">我们推荐显式初始化</span>

<span style="color:purple">注意 `MyStructure my_var{1, 2, 3};` 这样的 C++ 风格是不允许的</span>（见 getter setter）

默认初始化使得所有变量都会被初始化，即使没有给定初始化表达式。

```lumos
int   a; // 初始化为 0
float b; // 初始化为 0.0
bool  c; // 初始化为 false
char  d; // 初始化为 '\0'
str   e; // 初始化为 ""
```

变量类型与表达式类型不同时变量初始化被视为显式类型转换。但之后的赋值只允许隐式类型转换。

```lumos
int* a = 0x123456;
```

### 手动初始化

使用 `lateinit` 属性来让变量不自动初始化。<br>
<span style="color:green">注意访问未初始化的变量是未定义行为</span>

```lumos
@lateinit
int a;         // 此时 a 未初始化
initvar a = 1; // 手动初始化 a
println(a);    // 1
```

对于不可变变量，使用 `lateinit` 时仅可以在 `initvar` 处赋值一次。

```lumos
@lateinit
int val a;     // 此时 a 未初始化
initvar a = 1; // 手动初始化 a
println(a);    // 1
a = 2;         // error: 无法重新赋值给不可变变量
```

对于任意的变量，我们都能不断地重新初始化它。

```lumos
int val a = 1;
initvar a = 2;
initvar a = 3;
```

> 这给了我们一种合法的强行更改不可变变量的方法。

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

## 限定符

### `restrict`

<span style="color:green">注意 `restrict` 不是属性</span>

`restrict` 限定符用于指针，表示指针所指向的内存区域不会被其他指针访问。

```lumos
int* restrict a = malloc(4);
int* restrict b = a; // 这是不可以的
int*          c = a; // 这也是不可以的
```

### `volatile`

<span style="color:green">注意 `volatile` 不是属性</span>

`volatile` 限定符用于变量，表示变量可能会被其它线程或硬件改变，编译器不会对其访问（读写）进行优化。

```lumos
volatile int a = 1;
```

## 属性

### `register`

`@register(寄存器名)` 属性用于强制变量存储在寄存器中，而不是内存中。<br>
<span style="color:green">这会导致相应寄存器无法被其它变量使用</span><br>
<span style="color:purple">无特殊需求不应该使用</span>

```lumos
@register("rax")
int a = 1;
```

## `isrestrict` 运算符

`isrestrict` 运算符用于判断两个指针是否独立。

```lumos
int* a = malloc(4);
int* b = malloc(4);
int* c = a;

// 当编译器可以自动推断内存块大小时
println(isrestrict(a, b)); // 输出 true
println(isrestrict(a, c)); // 输出 false
// 当编译器不能自动推断内存块大小时
println(isrestrict(a, 4, b, 4)); // 输出 true
println(isrestrict(a, 4, c, 4)); // 输出 false
```

## getter setter

你可以使用 getter 和 setter 来访问变量。

```lumos
int a {
  \get {
    return 1;
  }
  \set {
    println("set a to $value");
  }
}

a = 2; // 输出 set a to 2
println(a); // 输出 1
```
