# 入门示例 {#title}

## 你好，世界 {#hello-world}

最简单的可运行程序，演示 `act` 与标准输出权限。`main` 使用 `act[io.out]` 显式声明输出权限，调用 `println` 时不会触发权限错误，体现副作用必须可审计的设计。
> 相关语法： [函数与纯度](../grammar/function.md#function-purity)，[副作用权限系统](../grammar/permission.md#title)，[标准库 IO](../std/io.md#title)。

```lumos: helloworld.lm```

## 你好，循环 {#hello-loop}

演示 `while`、经典 `for`、区间遍历三种循环写法。`while` 强调条件循环，经典 `for` 展示初始化/条件/更新三段式结构，区间遍历展示 `for (val v : 1 .. 3)` 的范围语法与只读迭代变量。
> 相关语法： [控制流](../grammar/controlflow.md#title)，[遍历循环](../grammar/controlflow.md#loops-for)。

```lumos: helloloop.lm```

## 你好，函数 {#hello-function}

演示函数定义与调用，以及模块函数的使用。示例通过 `use "std" -> std` 引入模块，再用 `std.println` 调用函数；同时展示多个 `act` 函数之间的调用与返回值处理。
> 相关语法： [函数与纯度](../grammar/function.md#function-purity)，[模块](../grammar/modules.md#import)，[标准库 IO](../std/io.md#title)。

```lumos: hellofunc.lm```

---

相关内容：入门指南见 [快速开始](../begin/index.md)。
