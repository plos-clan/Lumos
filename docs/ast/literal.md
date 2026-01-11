# 字面量 (Literal) {#title}

本页定义了 Lumos 支持的各类字面量结构。

## 字面量 `Literal` {#literal}

```rust
pub enum Literal {
    /// 整数 (支持不同进制)
    Int(i64),

    /// 浮点数
    Float(f64),

    /// 字符串 (UTF-8)
    String(String),

    /// 布尔值
    Bool(bool),

    /// 字符 (Unicode)
    Char(char),

    /// 空值/无类型
    Unit,

    /// 带有单位的数值 (10.5m, 100kg)
    Measured(f64, String),

    /// 数组字面量 $[1, 2, 3]
    Array(Vec<Expr>),

    /// 结构体/对象初始化列表 ${.a=1, .b=2}
    Object(Vec<(String, Expr)>),

    /// 原始字节字面量 b"hello"
    Bytes(Vec<u8>),
}
```

---

相关内容：字面量规则见 [词法](../lexical/index.md#title)。
