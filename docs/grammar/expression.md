# 表达式

<span style="color:purple">与 C++ 中不一样的是，在 lumos 中如果表达式是一个右值，则不能取地址。<br>常量表达式必为右值，故不能取地址。</span>

```lumos
var my_var = 10;
expression expr1 = my_var; // 左值
val a = &expr1; // 通过编译
expression expr2 = 10;     // 右值
val b = &expr2; // 错误
constexpr expr3 = 10;      // 右值
val c = &expr3; // 错误
```

## 表达式定义 `let`

`let` 用于声明一个表达式，其值在运行时确定，可以作为宏的替代。<br>
以下两种写法在使用 `int b = expr * 2;` 时等效，均展开为 `int b = (a + 10) * 2;`。

```lumos
#define expr (a + 10)
let expr = a + 10;
```

表达式也可以是可求值代码块：

```lumos
let expr = val {
    int x = a + 10;
    x * 2;
};
```

表达式拥有作用域，无需像宏那样担心重复定义的问题。

```lumos
{
    let expr = a + 10;
    int b = expr * 2;
} // expr 超出作用域，不再可用

{
#define expr (a + 10)
    int b = expr * 2;
#undef expr // 需要手动取消定义
}
```

表达式可以在作用域内重新定义，不会影响外部作用域。

```lumos
let expr = a + 10;
{
    let expr = a + 20;
    int b = expr * 2; // b = (a + 20) * 2
}
int b = expr * 2; // b = (a + 10) * 2
```

## 常量表达式定义 `constexpr`

`constexpr` 用于声明一个常量表达式，其值在编译时确定。

```lumos
constexpr expr = 10 + 1;
```

其余用法与表达式相同。
