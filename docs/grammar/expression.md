# 表达式

<span style="color:purple">
与 C++ 中不一样的是，在 Lumos 中如果表达式是一个右值，则不能取地址。<br />
常量表达式必为右值，故不能取地址。
</span>

```lumos
var i32 myvar = 10; // 变量
let expr1 = myvar;  // 左值
val a     = &expr1; // 通过编译
let expr2 = 10;     // 右值
val b     = &expr2; // 错误
lit expr3 = 10;     // 右值
val c     = &expr3; // 错误
```

## 表达式定义 `let`

`let` 用于声明一个表达式，其值在运行时确定，可以作为宏的替代。<br />
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

## 常量表达式定义 `lit`

`lit` 用于声明一个常量表达式，性质类似一个字面量，其值在编译时确定。

```lumos
lit expr = 10 + 1;
// 之后使用 expr 与使用字面量 11 等效
[expr]i32 a;
[11]i32 b;
// a 与 b 的大小完全相同
```

### 返回常量表达式的函数

```lumos
fn lit my_add(i32 a, i32 b) -> i32 {
    return a + b;
}
```

这里的参数可以省略 `lit` 关键字，其必须为常量表达式。

***其余用法与普通表达式相同。***

*明显不能出现指向表达式的指针 `let*`*
