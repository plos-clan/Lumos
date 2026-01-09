# 语句 (Statement)

本页定义了 Lumos 的语句结构 `Stat`。

## 语句 `Stat`

```rust
pub enum Stat {
    /// 表达式语句
    Expr(Expr),

    /// 代码块
    Block(Block),

    /// 条件分支
    If {
        /// 支持 (expr, expr, cond) 这种多条件形式
        conditions: Vec<Expr>,
        then_body: Box<Stat>,
        else_body: Option<Box<Stat>>,
    },

    /// 循环
    Loop {
        kind: LoopKind,
        init: Option<Box<Stat>>,
        cond: Option<Expr>,
        step: Option<Box<Stat>>,
        body: Box<Stat>,
        label: Option<String>,
    },

    /// 跳转
    Jump {
        kind: JumpKind,
        label: Option<String>,
        value: Option<Expr>, // 用于 return 返回值
    },

    /// 权限操作块 (act[+io] { ... })
    Permission {
        op: PermissionOp,
        perms: Vec<String>,
        body: Box<Stat>,
    },

    /// 静态假设 (assume <cond>)
    Assume(Expr),

    /// 声明语句 (在函数内部定义变量或局部函数)
    Decl(Decl),

    /// 异常处理
    Try {
        body: Block,
        catches: Vec<CatchBlock>,
        finally: Option<Block>,
    },

    /// 抛出异常
    Throw(Expr),

    /// 空语句
    Empty,
}
```

## 辅助枚举

### 循环类型 `LoopKind`

```rust
pub enum LoopKind {
    For,
    While,
    DoWhile,
    Infinite, // loop { ... }
}
```

### 跳转类型 `JumpKind`

```rust
pub enum JumpKind {
    Return,
    Break,
    Continue,
    Leave,
    Goto,
}
```

### 权限操作 `PermissionOp`

```rust
pub enum PermissionOp {
    Add,            // +xxx
    Sub,            // -xxx
    Reset,          // xxx (重置为仅有这些)
    Escape(String), // * (逃生舱，带理由)
}
```

### 异常捕获 `CatchBlock`

```rust
pub struct CatchBlock {
    pub error_var: Option<String>,
    pub error_type: TypeRef,
    pub body: Block,
}
```

---

相关内容：控制流语法见 [语法/控制流](../grammar/controlflow.md#control-flow)。
