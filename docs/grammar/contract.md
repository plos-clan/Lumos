# 契约（Contract） {#title}

契约用于定义类型必须满足的一组能力/接口。

被定义的对象可以是结构/类/枚举/模块等。

## 定义契约 {#define}

为结构/类/枚举定义契约：

```lumos
contract Hash {
    fun hash(Self) -> usize;
}
```

为模块定义契约：

```lumos
contract MyContract {
    class MyClass;
    fun my_func() -> u32;
}
```

## 实现契约 {#impl}

为结构/类/枚举实现契约：

```lumos
impl Hash for MyType {
    fun hash(self) -> usize { ... }
}
```

为模块实现契约：

```lumos
module my_module: MyContract {
    class MyClass { ... }
    fun my_func() -> u32 { ... }
}
```

## 约束使用 {#constraint}

```lumos
def process[T: Hash](T x) { ... }
```
