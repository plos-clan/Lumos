
# 异常机制

## 允许函数抛出异常

Lumos 默认不允许抛出异常，但可以用 `@throwable` 声明函数会抛出异常。

```lumos
@throwable
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

Lumos 允许在不可抛出异常的函数中调用可抛出异常的函数，只要程序员保证异常被处理。

```lumos
@throwable
fn my_func1() { // 这个函数会抛出异常
  throw "假如出现异常";
}

fn my_func() { // 这个函数不能抛出异常
  my_func1() catch {
    // 必须处理异常且不能再次抛出
  }
}
```

## 处理异常

`catch` 抓取 `try` 中出现的异常并处理。

```lumos
try {
  my_func1();
  my_func2();
} catch (Error &e) {
  处理逻辑
}
```

与其他许多语言不同的是，`try` 块并不是必须的。<br>
此时 `catch` 抓取上一个表达式的异常并处理。

```lumos
my_func();
catch (Error &e) {
  处理逻辑
}
```

上一个表达式末尾的分号可以不用添加。

```lumos
my_func() catch (Error &e) {
  处理逻辑
}
```

如果你不需要知道到底发生了什么异常，可以不在 `catch` 之后添加参数。

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

## 抛出异常

发生异常时请 `throw` 异常，能 `throw` 的类型必须是继承 `Error` 类的类型。<br>
<span style="color:green">能直接 `throw` 字符串是因为字符串能隐式转换成 `Error` 类型</span>

```lumos
throw "遇到异常"; // 字符串会自动转换为 Error 类型
throw Error("遇到异常");
```

也可以像调用函数那样写多个参数，会被自动打包。

```lumos
throw 1, 2, 3; // 会被打包成一个 Error 对象
throw Error(1, 2, 3);
```
