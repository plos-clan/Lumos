# 语法

批注

- *<span style="color:red">error</span>: <span style="color:darkcyan">应该抛出错误</span>*
- *<span style="color:orange">warning</span>: <span style="color:darkcyan">应该给出警告</span>*
- <span style="color:green">普通提示</span>
- <span style="color:purple">重要提示</span>

## 语法列举

```lumos
function(argument1, argument2)
function(argument1)
\def> function argument1
```

```lumos
i32 a = 1;     // 不可变变量
var i32 b = 2; // 可变变量
val c = 3;     // 推导不可变
var d = 4;     // 推导可变
let e = 5;     // 表达式
lit f = 6;     // 常量表达式
```

```lumos
// 函数
act[io.out] main(i32 argc, [[]string] argv) -> i32 {
    println("Hello, world!");
    return 0;
}
```

```lumos
// lambda 表达式
val squre = def (f32 a) -> f32 {
    return a * a;
};
```

```lumos
act[io.out] main() -> i32 {
    var i32 a = 1;
    var i32 b = 2;
    i32 c = a + b;
    println(`$a + $b = $c`); // 格式化字符串
    return 0;
}
```

```lumos
using "std";
```

```lumos
act[io.out] main() -> i32 {
    i32 a = 1;
    i32 b = 2;
    i32 c = a pow b;
    println(`$a ^ $b = $c`); // 格式化字符串
    return 0;
}
```

```lumos
&[10]i32
```
