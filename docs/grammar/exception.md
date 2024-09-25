
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

??? note "为什么默认不开启异常"
    为了避免异常被滥用，Lumos 默认不允许函数抛出异常。<br>
    许多大型项目都会关闭异常，因为异常会导致代码难以理解和维护。

## 处理异常

`catch` 抓取 `try` 的异常并处理。

```lumos
try {
  my_func1();
  my_func2();
} catch (Error &e) {
  cout e;
}
```

与其他许多语言不同的是，`try` 块并不是必须的。<br>
此时 `catch` 抓取上一个表达式的异常并处理。

```lumos
my_func();
catch (Error &e) {
  cout e;
}
```

简写，上一个表达式末尾的分号可以不用添加。

```lumos
my_func() catch (Error &e) {
  cout e;
}
```

空的 `catch` 块可以让程序忽略异常。<br>
<span style="color:purple">永远不要这么做</span>

```lumos
my_func() catch;
```

## 抛出异常

发生异常时请 `throw` 异常。

```lumos
throw E"遇到异常"
throw Error("遇到异常")
```

也可以像调用函数那样写多个参数。

```lumos
throw 1, 2, 3
throw(1, 2, 3)
```
