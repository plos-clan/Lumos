# 类型系统 (Type) {#title}

本页定义了 Lumos 的类型引用结构 `TypeRef`。

## 类型引用 `TypeRef` {#type-ref}

```rust
pub enum TypeRef {
    /// 基础类型 (如 i32, f64, bool, string, unit 等)
    Primitive(String),

    /// 数组类型 [T; N] 或 [T] (切片/动态数组)
    Array {
        element: Box<TypeRef>,
        size: Option<Box<Expr>>, // None 表示动态大小或切片
    },

    /// 指针类型 *T 或 *mut T
    Pointer {
        target: Box<TypeRef>,
        is_mutable: bool,
    },

    /// 引用类型 &T 或 &mut T
    Reference {
        target: Box<TypeRef>,
        is_mutable: bool,
    },

    /// 泛型实例化 Vec<T, Alloc>
    Generic {
        base: String,
        args: Vec<TypeRef>,
    },

    /// 带有计量单位的类型 (f64 as meter)
    Measure {
        base: Box<TypeRef>,
        unit: String,
    },

    /// 函数指针类型 (def(i32, i32) -> i32)
    Function {
        purity: Purity,
        params: Vec<TypeRef>,
        return_type: Box<TypeRef>,
    },

    /// 效应多态占位符 (%)
    EffectPlaceholder,

    /// 元组类型 (i32, string)
    Tuple(Vec<TypeRef>),

    /// 自动推断类型 (var x = 1)
    Inferred,
}
```

---

相关内容：类型系统见 [语法/类型](../grammar/type.md#types)。
