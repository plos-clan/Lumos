
# 异常机制 {#title}

## 允许抛出异常 {#allow-throw}

Lumos 默认不允许抛出异常。需要在函数签名的权限列表中声明 `exn`，表示该函数可能抛出并传播异常：

```lumos
act[exn] my_func() -> u32 {
    throw "测试异常"; // 正常抛出异常
}
```

```lumos
act my_func() -> u32 {
    throw "测试异常"; // 编译错误：函数默认不允许抛出异常
}
```

!!! note "为什么默认不开启异常"
    为了避免异常被滥用，Lumos 默认不允许函数抛出异常。  
    许多大型项目都会关闭异常，因为异常会导致代码难以理解和维护。

Lumos 允许在不可传播异常的函数中调用 `act[exn]` 函数，只要程序员在函数中手动处理所有异常。

```lumos
act[exn, io.out] my_func1() -> u32 { // 这个函数会抛出异常
    throw "假如出现异常";
}

act[io.out] my_func() -> i32 { // 这个函数不能传播异常
    my_func1() or {
        // 必须处理异常且不能再次抛出
        println("Error!");
        return -1;
    }
    return 0;
}
```

## 两种错误处理机制 {#error-mechanisms}

Lumos 提供两种正交的错误处理机制，适用于不同场景：

| 机制 | 语义 | 适用场景 |
|------|------|----------|
| `-> T or E` | 错误作为返回值，无栈展开 | `def`/`fun`/`act` 均可用；错误是计算结果的一部分 |
| `act[exn]` | 异常可穿越函数边界（栈展开传播） | 仅 `act` 可用；用于确实属于"意外"的运行时情况 |

两者完全独立，不存在矛盾：`-> T or E` 描述返回类型，`act[exn]` 描述传播行为。

```lumos
// 值级错误——fun 可用，错误作为返回值，无异常传播
fun divide(i32 a, i32 b) -> i32 or DivisionError {
    if (b == 0) return DivisionError("除零");
    return a / b;
}

// 异常传播——需要 act[exn]，用于真正的意外情况
act[exn, fs.read] read_config(string path) -> Config {
    val file = open(path) or throw FileNotFoundError(path);
    // ...
}
```

## 处理异常 {#handle-exceptions}

与大多数语言中的 `try-catch` 类似，在 Lumos 中可以使用 `or` 来抓取 `try` 中出现的异常并处理。

```lumos
try {
    my_func1();
    my_func2();
} or (e as Error) {
    处理逻辑
}
```

与其他许多语言不同的是，`or` 块并不是必须的。  
此时 `or` 抓取左侧表达式的异常并处理。

```lumos
my_func() or (e as Error) {
    处理逻辑
}
```

如果你不需要知道到底发生了什么异常，可以不在 `or` 之后添加参数。  
`or` 后的内容就是通用的异常处理逻辑。

```lumos
my_func() or {
    处理逻辑
}
```

处理逻辑可以直接是 `break` `continue` `return` 等控制流语句。

```lumos
my_func() or return -1;
```

空的 `or` 块可以让程序忽略异常并继续执行。  
<span style="color:purple">不要随便这么做</span>

```lumos
my_func() or;
```

## 自动处理异常 {#auto-handle}

Lumos 支持在函数调用时自动处理异常。  
通过在外层函数声明时附加 `@exception(panic)` 可以自动在内部触发异常时打印错误信息并终止程序，相当于对函数体内所有 `act[exn]` 调用隐式添加 `or panic`。  
`@exception(bypass)` 和 `@exception(return)` 是对 `or;` / `or return 默认值;` 的便捷替代，但推荐直接使用显式 `or` 块以保持清晰性。

```lumos
act[exn] this_func_will_throw() {
    throw "测试异常";
}

@exception(panic)
act panic_when_throw() {
    this_func_will_throw();
    // 当 this_func_will_throw() 抛出异常时，程序会打印错误信息并终止
}

@exception(bypass)
act bypass_when_throw() {
    this_func_will_throw();
    // 当 this_func_will_throw() 抛出异常时，程序会忽略异常并继续执行
}

@exception(return)
act return_when_throw() -> i32 {
    this_func_will_throw();
    // 当 this_func_will_throw() 抛出异常时，程序会返回默认值 0
}
```

```lumos
@exception(return, 10)
act return_when_throw() -> i32 {
    this_func_will_throw();
    // 当 this_func_will_throw() 抛出异常时，程序会返回默认值 10
}
```

## 抛出异常 {#throw}

发生异常时请 `throw` 异常，能 `throw` 的类型必须是继承 `Error` 类的类型。  
<span style="color:green">能直接 `throw` 字符串是因为字符串能隐式转换成 `Error` 类型</span>  
<span style="color:green">`StrError` `I32Error`</span>

```lumos
throw "遇到异常"; // 字符串会自动转换为 Error 类型
throw Error("遇到异常");
```

也可以像调用函数那样写多个参数，会被自动打包。

```lumos
throw 1, 2, 3; // 会被打包成一个 Error 对象
throw Error(1, 2, 3);
```

`Error` 类型会自动将传入参数全部打包到一个字符串内。

## 异常导致的 panic {#panic}

程序由自动异常处理触发 panic 时，Lumos 会打印错误信息：

```text
panic: function 'xxx' threw an exception.
  Error: 测试异常

stack trace:
  at my_func() in test.lm:9
  at main() in test.lm:15
```

---

相关内容：控制流见 [控制流](controlflow.md#title)。
