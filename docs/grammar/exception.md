
# 异常处理

```
try {
  my_func1();
  my_func2();
} catch (Error &e) {
  cout e;
}
```

`catch` 抓取 `try` 的异常并处理。

与其他许多语言不同的是，`try` 块并不是必须的。<br>
此时 `catch` 抓取上一个表达式的异常并处理。

```
my_func();
catch (Error &e) {
  cout e;
}
```

简写，上一个表达式末尾的分号可以不用添加。

```
my_func() catch (Error &e) {
  cout e;
}
```

空的 `catch` 块可以让程序忽略异常。<br>
<span style="color:purple">永远不要这么做</span>

```
my_func() catch;
```

发生异常时请 `throw` 异常。

```
throw E"遇到异常"
throw Error("遇到异常")
```

也可以像调用函数那样写多个参数。

```
throw 1, 2, 3
throw(1, 2, 3)
```

就像所有 lumos 语句一样，`throw` 也是可求值的，其值恒为 `void`，但你永远也没法获取到 `throw` 求得的值