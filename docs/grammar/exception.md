
# 异常机制

## 允许抛出异常

Lumos 默认不允许抛出异常，但可以用 `@exception(allow)` 声明函数可以抛出异常。<br>
也可以使用 `@exception(allow)` 声明命名空间或类中的所有函数都可以抛出异常。

```lumos
@exception(allow)
fn my_func() {
  throw "测试异常"; // 正常抛出异常
}
```

```lumos
fn my_func() {
  throw "测试异常"; // 编译错误：函数默认不允许抛出异常
}
```

!!! note "为什么默认不开启异常"
    为了避免异常被滥用，Lumos 默认不允许函数抛出异常。<br>
    许多大型项目都会关闭异常，因为异常会导致代码难以理解和维护。

Lumos 允许在不可抛出异常的函数中调用可抛出异常的函数，只要程序员在函数中手动处理所有异常。

```lumos
@exception(allow)
fn my_func1() { // 这个函数会抛出异常
  throw "假如出现异常";
}

fn my_func() -> int { // 这个函数不能抛出异常
  my_func1() catch {
    // 必须处理异常且不能再次抛出
    println("Error!");
    return -1;
  }
  return 0;
}
```

## 禁止异常穿过

使用 `@exception(allow)` 声明函数可以抛出异常后，Lumos 会同时允许异常穿过函数边界。<br>
如果需要禁止异常穿过函数边界，应该使用 `Type or Error` 作为函数的返回类型。<br>
<span style="color:green">注意 `Type or Error` 返回类型和 `@exception(allow)` 不能同时使用</span> ??既禁止又允许什么鬼嘛??

```lumos
fn my_func(a as int) -> int or Error {
  if (a < 0) {
    throw "参数不能小于 0"; // 抛出异常
  }
  return a * 2; // 返回正常值
}

fn main {
  var result = my_func(-1) or {
    println("发生异常，无法继续执行");
    return -1; // 处理异常
  };
  println("结果是: " + result);
}
```

## 处理异常

`catch` 抓取 `try` 中出现的异常并处理。

```lumos
try {
  my_func1();
  my_func2();
} catch (e as Error) {
  处理逻辑
}
```

与其他许多语言不同的是，`try` 块并不是必须的。<br>
此时 `catch` 抓取上一个表达式的异常并处理。

```lumos
my_func();
catch (e as Error) {
  处理逻辑
}
```

上一个表达式末尾的分号可以不用添加。

```lumos
my_func() catch (e as Error) {
  处理逻辑
}
```

如果你不需要知道到底发生了什么异常，可以不在 `catch` 之后添加参数。<br>
`catch` 后的内容就是通用的异常处理逻辑。

```lumos
my_func() catch {
  处理逻辑
}
```

空的 `catch` 块可以让程序忽略异常并继续执行。<br>
<span style="color:purple">永远不要这么做</span>

```lumos
my_func() catch;
```

## 自动处理异常

Lumos 支持在函数调用时自动处理异常。<br>
通过在外层函数声明时附加 `@exception(panic)` 可以自动在内部出发异常时打印错误信息并终止程序。<br>
通过在外层函数声明时附加 `@exception(bypass)` 可以自动在内部出发异常时忽略异常并继续执行。（若函数有返回值则使用其类型的默认值）<br>
通过在外层函数声明时附加 `@exception(return)` 可以自动在内部出发异常时使当前函数返回默认值。

```lumos
fn this_func_will_throw() {
  throw "测试异常";
}

@exception(panic)
fn panic_when_throw() {
  this_func_will_throw();
  // 当 this_func_will_throw() 抛出异常时，程序会打印错误信息并终止
}

@exception(bypass)
fn bypass_when_throw() {
  this_func_will_throw();
  // 当 this_func_will_throw() 抛出异常时，程序会忽略异常并继续执行
}

@exception(return)
fn return_when_throw() -> int {
  this_func_will_throw();
  // 当 this_func_will_throw() 抛出异常时，程序会返回默认值 0
}
```

```lumos
@exception(return, 10)
fn return_when_throw() -> int {
  this_func_will_throw();
  // 当 this_func_will_throw() 抛出异常时，程序会返回默认值 10
}
```

## 抛出异常

发生异常时请 `throw` 异常，能 `throw` 的类型必须是继承 `Error` 类的类型。<br>
<span style="color:green">能直接 `throw` 字符串是因为字符串能隐式转换成 `Error` 类型</span><br>
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

## 异常导致的 panic

程序由自动异常处理触发 panic 时，Lumos 会打印错误信息：

```text
panic: function 'xxx' threw an exception.
  Error: 测试异常

stack trace:
  at my_func() in test.lm:9
  at main() in test.lm:15
```
