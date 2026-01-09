# 表达式 (Expression)

本页定义了 Lumos 的表达式结构 `Expr`。

## 表达式 `Expr`

```rust
pub enum Expr {
    /// 字面量 (1, "hello", true 等)
    Literal(Literal),

    /// 符号引用 (变量名、函数名、命名空间等)
    Symbol(String),

    /// 二元运算 (a + b, a && b)
    Binary {
        op: BinaryOp,
        left: Box<Expr>,
        right: Box<Expr>,
    },

    /// 一元运算 (-a, !b, a++)
    Unary {
        op: UnaryOp,
        operand: Box<Expr>,
        is_postfix: bool,
    },

    /// 函数调用 (foo(a, b))
    Call {
        callee: Box<Expr>,
        args: Vec<Expr>,
    },

    /// 成员访问 (obj.member)
    Member {
        object: Box<Expr>,
        member: String,
    },

    /// 下标访问 (arr[index])
    Index {
        target: Box<Expr>,
        index: Box<Expr>,
    },

    /// 三元运算 (cond ? a : b)
    Ternary {
        cond: Box<Expr>,
        then_expr: Box<Expr>,
        else_expr: Box<Expr>,
    },

    /// 代码块表达式 (val x = { ... })
    BlockExpr(Block),

    /// Lambda 表达式 (def(x) -> x + 1)
    Lambda(Box<FunctionDecl>),

    /// 表达式别名 (let expr = ...)
    LetAlias {
        name: String,
        value: Box<Expr>,
    },

    /// 编译期常量别名 (lit expr = ...)
    LitAlias {
        name: String,
        value: Box<Expr>,
    },

    /// 调试伪函数 (dbg(...))
    Dbg(Vec<Expr>),

    /// 类型转换 (expr as T)
    Cast {
        expr: Box<Expr>,
        target_type: TypeRef,
    },

    /// 类型判断 (expr is T 或 expr is state)
    Is {
        expr: Box<Expr>,
        target: IsTarget,
    },

    /// 异步等待 (await expr)
    Await(Box<Expr>),

    /// 单位转换 (expr in unit)
    UnitConvert {
        expr: Box<Expr>,
        target_unit: String,
    },
}
```

## 运算符枚举

### 二元运算符 `BinaryOp`

```rust
pub enum BinaryOp {
    Add, Sub, Mul, Div, Mod,      // 算术
    Eq, Ne, Lt, Le, Gt, Ge,       // 比较
    And, Or, Xor,                 // 逻辑/位运算
    Shl, Shr,                     // 移位
    Assign,                       // 赋值 (=)
    AddAssign, SubAssign, ...     // 复合赋值
    Pipeline,                     // 管道 (|>)
}
```

### 一元运算符 `UnaryOp`

```rust
pub enum UnaryOp {
    Pos, Neg,      // + -
    Not, BitNot,   // ! ~
    Inc, Dec,      // ++ --
    Deref,         // * (解引用)
    AddrOf,        // & (取地址)
}
```

### 类型判断目标 `IsTarget`

```rust
pub enum IsTarget {
    Type(TypeRef),
    State(String), // 如 is empty, is valid
}
```

---

相关内容：表达式语法见 [语法/表达式](../grammar/expression.md)。
