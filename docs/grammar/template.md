
# 模板

我们使用 `@<xxx>` 来定义一个模板。

```lumos
@<typename T>
fn my_func(T arg1) {
    println(arg1);
}
```

我们可以这样使用上述函数

```lumos
fn main() {
    my_func(123);
    my_func("abc");
}
```

我们可以对模板进行特化。

```lumos
@<typename T>
fn my_func(T arg1) {
    println(arg1);
}

@<int>
fn my_func(T arg1) {
    println(`整数：$arg1`);
}
```

对模板进行显式的实例化。

```lumos
fn my_func<int>;
```
