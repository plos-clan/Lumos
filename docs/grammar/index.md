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
\fn> function argument1
```

```lumos
var a = 1; // 变量
val b = 2; // 不可变变量
let c = 3; // 表达式
lit d = 4; // 常量表达式
```

```lumos
// 函数
fn main(i32 argc, *[argc, *byte] argv) -> i32 {
    println("Hello, world!");
    return 0;
}
```

```lumos
// lambda 表达式
var squre = fn (float a) {
    return a * a;
}
```

```lumos
fn main {
    var a = 1;
    var b = 2;
    var c = a + b;
    println(`$a + $b = $c`); // 格式化字符串
    return 0;
}
```

```lumos
using "std";
```

```lumos
fn main {
    var a = 1;
    var b = 2;
    var c = a pow b;
    println(`$a ^ $b = $c`); // 格式化字符串
    return 0;
}
```

```lumos
&[10, i32]
```
