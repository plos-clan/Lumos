
# 模板

我们使用 `@<xxx>` 来定义一个模板。

```lumos
fn my_func<typename T>(T arg1) {
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

我们可以对模板进行特化。<br>
<span style="color:green">注意特化必须在模板声明后</span>

```lumos
fn my_func<typename T>(T arg1) {
    println(arg1);
}

fn my_func<int>(T arg1) {
    println(`整数：$arg1`);
}
```

<span style="color:green">注意模板在实例化时才会检查，也就是模板如果不使用程序员可能不会发现其中的语法错误。</span><br>
因此我们建议对模板进行完整的单元测试。

```lumos
fn my_func<typename T>(T arg1) {
    println(arg1 << 1); // 由于不知道 T 的类型，无法检查是否能够左移
}

fn main {
    my_func(123);     // 整数可以左移，通过编译
    my_func(123.456); // 浮点数不能左移，这里会报错
    return 0;
}
```

对模板进行显式的实例化。

```lumos
fn my_func<int>;
```
