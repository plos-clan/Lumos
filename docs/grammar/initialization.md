
# 初始化

所有变量默认都会初始化，以下是一些内置类型的默认初始化值：

```lumos
int   a; // 自动初始化为 0
float b; // 自动初始化为 0.0
bool  c; // 自动初始化为 false
char  d; // 自动初始化为 '\0'
str   e; // 自动初始化为 ""
```

## 手动初始化

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
