
# 模板

## 带类型

我们使用 `</xxx/>` 来定义一个模板。

```lumos
fn</T/> my_func(arg1 as T) {
    println(`${typenameof T}: $arg1`);
}
```

我们可以这样使用上述函数

```lumos
fn main() {
    my_func(123); // i32: 123
    my_func(123.456); // f64: 123.456
    my_func("abc"); // str: abc
}
```

我们可以对模板进行特化。<br />
<span style="color:green">注意特化必须在模板声明后</span>

```lumos
fn</T/> my_func(arg1 as T) {
    println(arg1);
}

fn my_func</int/>(arg1 as T) {
    println(`整数：$arg1`);
}
```

<span style="color:green">注意模板在实例化时才会检查，也就是模板如果不使用程序员可能不会发现其中的语法错误。</span><br />
因此我们建议对模板进行完整的单元测试。

```lumos
act</T/> my_func(arg1 as T) {
    println(arg1 << 1); // 由于不知道 T 的类型，无法检查是否能够左移
}

act main {
    my_func(123);     // 整数可以左移，通过编译
    my_func(123.456); // 浮点数不能左移，这里会报错
    return 0;
}
```

也可以手动对其类型进行限制：

```lumos
@staticassert(T !is floattype)
act my_func</typename T/>(arg1 as T) {
    println(arg1 << 1); // 由于不知道 T 的类型，无法检查是否能够左移
}
```

对模板进行显式的实例化。

```lumos
act my_func</int/>;
```

## 不带类型

使用以下语法定义一个模板：

```lumos
template my_add(a, b) {
    return a + b;
}
```

这种写法只在“调用”处进行展开并执行类型检查，模板本身并不会被执行任何检查，只要它的语法正确即可。

```lumos
val x = add(1, 2);               // 推导为 int + int
val y = add(1.0, 2.5);           // 推导为 float + float
val z = add("Hello, ", "world"); // 推导为字符串拼接
```

`template` 的参数建议默认执行懒拷贝策略，即必须拷贝时则拷贝，不过也可以提前拷贝所有参数。

在参数名前加 `*` 执行强制拷贝，加 `&` 执行强制引用。

```lumos
template my_template_copy(*a) {
    a = 2;
}
template my_template_ref(&a) {
    a = 2;
}
act main {
    var x = 1;
    my_template_copy(x); // x 被拷贝，内部 x 变为 2
    println(x);          // 输出 1

    var y = 1;
    my_template_ref(y);  // y 被引用，内外 y 变为 2
    println(y);          // 输出 2
}
```
