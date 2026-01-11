
# 括号 {#title}

## 小括号 {#parentheses}

### 参数列表 {#list}

小括号用于表示参数列表。

```lumos
act[io.out] my_func1() -> unit {
    println("Hello, world!");
}
act[io.out] my_func2(i32 a, i32 b) -> unit {
    println(a + b);
}
```

在不会引起歧义的情况下，可以省去小括号。

```lumos
act[io.out] my_func1 -> unit {
    println "Hello, world!";
}
```

### 函数调用 {#function-call}

小括号用于表示函数调用。

```lumos
act[io.out] my_func() -> unit {
    println("Hello, world!");
}

my_func();
```

在不会引起歧义的情况下，可以省去小括号。

```lumos
act[io.out] my_func -> unit {
    println "Hello, world!";
}

my_func;
```

## 中括号 {#square-brackets}

### 索引 {#indexing}

中括号用于表示索引。

```lumos
val a = $[1, 2, 3, 4, 5];
println(a[0]);
```

```lumos
val a = $[
    $[1, 2, 3, 4, 5],
    $[6, 7, 8, 9, 10],
];
println(a[0][0]); // 可以使用多个中括号
println(a[0, 0]); // 也可以使用逗号分隔
```

### 列表初始化 {#list-init}

中括号配合前置美元符号 `$[]` 用于表示数组或列表的初始化。

```lumos
var $[1, 2, 3, 4, 5] // 可变列表
val $[1, 2, 3, 4, 5] // 不可变列表
```

在不会引起歧义的情况下，可以省去 `val` 关键字。

```lumos
// 可以省去的情况，定义一个不可变列表
val a = $[1, 2, 3, 4, 5];
```

## 大括号 {#braces}

### 代码块 {#block}

大括号用于表示代码块。

### 结构体初始化 {#struct-init}

大括号配合前置美元符号 `${}` 用于表示结构体或类的初始化。

```lumos
val p = ${.x = 10, .y = 20};
```

### 矩阵 {#matrix}

矩阵是列表的一种扩展形式，使用类似 matlab 的语法。

```lumos
val a = $[
    1, 2, 3; // 以分号分隔行
    4, 5, 6;
    7, 8, 9;
];

println(a[0, 0]); // 1
println(a[1, 1]); // 5
println(a[2, 2]); // 9
```

## 大括号 {#braces-detail}

### 代码块 {#braces-detail-block}

大括号用于表示代码块。  
当大括号内直接存在 `;` 时，判定为代码块。

```lumos
if (true) {
    println("Hello, world!");
}
```

### 映射 {#map}

大括号用于表示映射。映射中的元素由键值对组成。  
映射的可变与不可变由大括号前的 `var` `val` 关键字决定。

```lumos
var ${ "a": 1, "b": 2, "c": 3 } // 可变映射
val ${ "a": 1, "b": 2, "c": 3 } // 不可变映射
```

在不会引起歧义的情况下，可以省去 `val` 关键字。

```lumos
// 可以省去的情况，定义一个不可变映射
val a = ${ "a": 1, "b": 2, "c": 3 };
```

### 结构体 {#struct}

大括号用于表示结构体初始化。

```lumos
Point p = ${.x = 1, .y = 2 };
```

在指定所有字段名称的情况下，可以隐式创建一个临时结构体类型。

```lumos
val p = ${.x = 1, .y = 2 }; // 创建一个临时结构体类型并创建变量 p
                           // 注意临时的结构体没有也不能添加任何成员函数
val p = ${.x = 1, 2 }; // 这样是错误的，因为没指定所有字段的名称
```

---

相关内容：表达式语法见 [表达式](expression.md#title)。
