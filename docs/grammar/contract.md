# 契约（Contract） {#title}

契约用于定义类型必须满足的一组能力/接口。

## 定义契约 {#define}

```lumos
contract Hash {
    fun hash(Self) -> usize;
}
```

## 实现契约 {#impl}

```lumos
impl Hash for MyType {
    fun hash(self) -> usize { ... }
}
```

## 约束使用 {#constraint}

```lumos
def process[T: Hash](T x) { ... }
```