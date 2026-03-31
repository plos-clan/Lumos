# 表达式 {#title}

关键字列表与用法速查请参阅 [词法/关键字](../lexical/keyword.md#title)。

对表达式取地址会导致其在栈上生成一个临时变量来存储这个值。

## 表达式定义 `let` {#let}

`let` 用于声明一个表达式，其值在运行时确定，可以作为宏的替代。  
以下两种写法在使用 `i32 b = expr * 2;` 时等效，均展开为 `i32 b = (a + 10) * 2;`。

```lumos
#define expr (a + 10)
let expr = a + 10;
```

表达式也可以是可求值代码块：

```lumos
let expr = val {
    var i32 x = a + 10;
    x * 2
};
```

表达式可以指定其类型（对其进行强制转换）：

```lumos
f32 expr = a + 10;
// 等效于
let expr = (a + 10) as f32;
```

表达式拥有作用域，无需像宏那样担心重复定义的问题。

```lumos
{
    let expr = a + 10;
    i32 b    = expr * 2;
} // expr 超出作用域，不再可用

{
#define expr (a + 10)
    var i32 b = \num> expr * 2;
#undef expr // 宏需要手动取消定义
}
```

表达式可以在作用域内重新定义，不会影响外部作用域。

```lumos
let expr = a + 10;
{
    let expr     = a + 20;
    i32 b = expr * 2; // b = (a + 20) * 2
}
i32 b = expr * 2;     // b = (a + 10) * 2
```

## 常量表达式定义 `lit` {#lit}

`lit` 用于声明一个常量表达式，性质类似一个字面量，其值在编译期确定。

```lumos
lit expr = 10 + 1;
// 之后使用 expr 与使用字面量 11 等效
[expr]i32 a;
[11]i32 b;
// a 与 b 的大小完全相同
```

### 返回常量表达式的函数 {#expression}

```lumos
@lit
def my_add(i32 a, i32 b) -> i32 {
    return a + b;
}
```

这里的参数可以省略 `lit` 关键字，其必须为常量表达式。

***其余用法与普通表达式相同。***

*明显不能出现指向表达式的指针 `let*`*

## 可空传播 `?` 与 `: fallback` {#nullable-propagation}

后缀运算符 `?` 用于标记一个可空表达式。当 `?` 标记的值为空（`null` 或 `none`）时，整个含 `?` 的前方完整表达式跳转到 `:` 后的后备值。

`:` 修饰**前方整个表达式**。复杂情况建议使用括号显式控制范围。

```lumos
my_func(a?) : b         // 若 a 为空则返回 b，否则返回 my_func(a)
my_func(a?, c?) : b     // 若 a 或 c 任意一个为空则返回 b，否则返回 my_func(a, c)
my_func(a?) + 1 : b     // 若 a 为空则返回 b，否则返回 my_func(a) + 1
(my_func(a?) : b) + 1   // 括号显式控制范围：先解决空值，再加 1
```

`?` 同时进行**类型窄化**：传递给函数时参数类型从 `T?` 窄化为 `T`，无需显式解包。

等价展开：

```lumos
my_func(a?, c?) : b
// 等价于
if (a == null or c == null) b else my_func(a, c)
```

## 类型与状态判断 `is` {#is}

`is` 用于判断一个值是否为某个类型，或对象是否处于某个状态。

```lumos
if (a is i32) {
    println("a is an integer");
}
```

状态判断通过对象中定义的 `is_状态名` 变量或函数实现。

```lumos
if (a is empty) {
    println("a is empty");
}

if (a.is_empty) {
    println("a is empty");
}
```

---

相关内容：类型转换规则见 [类型系统](type.md#types)。
