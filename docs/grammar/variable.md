
# 变量

## 变量声明 `var`

使用 `var` 关键字声明变量。

和绝大多数编程语言类似，直接：

```lumos
var 变量名 = 初始化表达式;
var a = 1;
```

即可完成变量的声明与初始化，变量的类型会被自动设置为右侧表达式的类型。

---

也可以在声明时指定类型：

```lumos
var 变量名 as 类型;
var 变量名 as 类型 = 初始化表达式;
var a as int;
var b as int = 1;
```

---

声明多个变量属于不同的类型：

```lumos
var a as int = 1, b as float = 2;
```

---

声明多个变量属于同一个类型：

```lumos
var a, b, c as int;
var a, b as int, c, d as float;
```

此时不能初始化。

---

声明指向不可变内存的不可变指针：

```lumos
val a as *int = 0x12345678;
a = null; // error: 无法将 null 赋值给不可变指针
a[0] = 1; // error: 无法修改不可变内存
```

声明指向可变内存的不可变指针：

```lumos
val a as *var int = 0x12345678;
a = null; // error: 无法将 null 赋值给不可变指针
a[0] = 1; // success
```

---

非空指针：

```lumos
var a as &int = 0x12345678;
var b as &int = null; // error: 无法将 null 赋值给非空指针
```

---

唯一引用：

```lumos
var a as !int = 0x12345678;
var b as !int = null; // error: 无法将 null 赋值给唯一引用
```

---

类型值的断言：

```lumos
var a as int ($ > 0) = 1;
var b as int ($ > 0) = 0; // error: 无法将 0 赋值给 int($ > 0)
```

此处 `$` 代表当前值，每次写入时都会进行断言。

---

***默认的规则***

对于一个变量，如果没有进行任何形式的取地址，那么它绝对不会被外部意外修改。

```lumos
var a = 1;
while (a > 0) {
  // Do nothing
}
```

这应当是一个死循环。

---

***允许的优化***

对于一个变量，取到的值可以是：

- 编译时能够确定的值（非 `volatile`）<br />
  *<span style="color:orange">warning</span>: <span style="color:darkcyan">对于编译期可确定的值使用 `lit`</span>*
- 上次修改后缓存的值（非 `volatile`）
- 对应内存地址当前的值

## 不可变变量声明 `val`

使用 `val` 关键字声明不可变变量，使用方法与 `var` 相同。

```lumos
val 变量名 = 初始化表达式;
val a = 1;
```

---

常量设计上在整个生命周期中不可变，但实际上可以强行修改，但进行修改时可能会因编译器的优化策略导致不可预测的结果。

```lumos
val a = 1;     // 定义常量
println(a);    // 输出 1
*(&a as *int) = 2; // 强行修改为 2
println(a);    // 由于编译器的优化策略，可能输出 1 或 2
```

??<span style="color:purple">这么做的见一个打一个</span>??

***允许的优化***

对于一个常量，取到的值可以是：

- 编译时能够确定的值（非 `volatile`）<br />
  *<span style="color:orange">warning</span>: <span style="color:darkcyan">对于编译期可确定的值使用 `lit`</span>*
- 作用域内任意位置缓存的值（非 `volatile`）
- 对应内存地址当前的值

## 初始化

Lumos 允许的初始化方式有：

- 赋值初始化 `int my_var = 1;`
- 构造函数初始化 `int my_var(1);`<br />
  <span style="color:green">对于基本数据类型有伪构造函数</span>
- 结构体元素赋值初始化 `var my_var as MyStructure = {1, 2, .third = 3};`
- 数组元素赋值初始化 `var my_var[] as int = {1, 2, [2] = 3};`
- 默认初始化 `var my_var as int;`<br />
  <span style="color:green">默认初始化会将基本数据类型变量初始化为二进制 0，对于其它数据类型则调用默认构造函数</span>
  <span style="color:green">我们推荐显式初始化</span>

<!-- <span style="color:purple">注意 `MyStructure my_var{1, 2, 3};` 这样的 C++ 风格是不允许的</span>（见 getter setter） -->

默认初始化使得所有变量都会被初始化，即使没有给定初始化表达式。

```lumos
val a as int; // 初始化为 0
float b; // 初始化为 0.0
bool  c; // 初始化为 false
char  d; // 初始化为 '\0'
str   e; // 初始化为 ""
```

变量类型与表达式类型不同时变量初始化被视为显式类型转换。但之后的赋值只允许隐式类型转换。

```lumos
int* a = 0x123456;
```

### 延迟初始化

使用 `lateinit` 作为初始值来让变量不自动初始化。<br />
<span style="color:green">注意访问未初始化的变量是未定义行为</span>

```lumos
var a as int = lateinit; // 此时 a 未初始化
a = 1; // 手动初始化 a
println(a);    // 1
```

对于不可变变量，使用 `lateinit` 时仅可以赋值一次。

```lumos
val a as int = lateinit; // 此时 a 未初始化
a = 1;                   // 手动初始化 a
println(a);              // 1
a = 2;                   // error: 无法重新赋值给不可变变量
```

```lumos
val a, b, c as int = lateinit;
if (xxx) {
  a = 1;
  b = 2;
  c = 3;
} else {
  a = 4;
  b = 5;
  c = 6;
}
```

### 懒初始化

使用 `lateinit` 接代码块作为初始值来让变量在第一次访问时自动初始化。<br />
代码块将延迟到第一次访问时执行，且只会执行一次。<br />
<span style="color:green">注意这种情况下不能连续声明多个变量</span>

```lumos
var a as int = lateinit {
  return 1;
} /* 此处语句已结束，不能接着声明变量 */
println(a); // 此时 a 被初始化为 1
```

代码块中可以使用外部的变量，但注意变量的值为代码块执行时的值，而非声明时的值。

```lumos
var a as int = 1;
var b as int = lateinit {
  return a;
}
a = 2;
println(b); // 输出 2
a = 3;
println(b); // 输出 2
```

## 全局不可变变量

你不应该声明一个全局的不可变变量，应该用 `let`(表达式) 或 `lit`(常量表达式) 代替。

见 [表达式](expression.md)。

## 临时变量 `with`

使用 `with` 关键字声明临时变量。

```lumos
with 变量名 = 初始化表达式; {
  // 此处可以引用临时变量
}

with 变量名 = 初始化表达式;
if 条件表达式 {
  // 此处可以引用临时变量
}

with 变量名 = 初始化表达式;
if 条件表达式 {
  // 此处可以引用临时变量
} else {
  // 此处可以引用临时变量
}
```

临时变量的作用域仅限于下一个表达式或代码块内，表达式或代码块结束后临时变量会被销毁。

`with` 后可以接 `val` 或 `var` 关键字来声明不可变或可变的临时变量。

```lumos
with var a = 1; {
  println(a); // 输出 1
}

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
val a as volatile int = 1;
```

## 属性

### `register`

`@register(寄存器名)` 属性用于强制变量存储在寄存器中，而不是内存中。<br />
<span style="color:green">这会导致相应寄存器无法被其它变量使用</span><br />
<span style="color:purple">无特殊需求不应该使用</span>

```lumos
@register("rax")
var a as int = 1;
```

注意此处的 `rax` 填写 `eax` `ax` `al` 都是可以的，实际宽度由类型决定。

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

## getter/setter

你可以使用 getter 和 setter 来访问变量。

```lumos
var a by int {
  \get { // 只能有一个 \get 但可以有多个 \set
    return 1;
  }
  \set { // 不写参数默认为 value
    println(`set a to $value`);
  }
}

a = 2;      // 输出 set a to 2
println(a); // 输出 1
```

可以不声明类型：

```lumos
var a by {
  \get -> int {
    return 1;
  }
  \set(value as int) {
    println(`set a to $value`);
  }
}
```

可以在内部声明变量来存储值：

```lumos
var a by int {
  var real_a as int = 1;
  \get {
    return real_a;
  }
  \set {
    println(`set a to $value`);
    real_a = value;
  }
}

a = 2;      // 输出 set a to 2
println(a); // 输出 2
```

外部无法访问到 `real_a` 变量。
