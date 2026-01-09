# 基础元数据与枚举 (Common)

本页定义了 AST 中通用的基础结构、位置信息以及各类修饰符枚举。

## 源代码位置

```rust
/// 源代码中的具体位置
pub struct SourceLocation {
    pub file: String,
    pub line: u32,
    pub column: u32,
}

/// 源代码中的一段范围
pub struct Span {
    pub start: SourceLocation,
    pub end: SourceLocation,
}
```

## 函数相关枚举

### 纯度等级 (Purity)

```rust
pub enum Purity {
    Def, // 纯函数：无副作用，确定性
    Fun, // 逻辑纯函数：允许内部副作用，对外透明
    Act, // 副作用函数：允许 IO 和全局状态修改
}
```

### 函数修饰符 (Modifier)

```rust
pub enum Modifier {
    Unsafe,     // 允许在 fun 中执行 act 操作
    Async,      // 异步函数
    OnceStrict, // once! act (再次调用报错)
    OnceSoft,   // once? act (再次调用忽略)
}
```

### 成员访问权限修饰符 (MemberModifier)

用于类或结构体成员函数，标记对 `self` 的访问权限。

```rust
pub enum MemberModifier {
    ReadOnly,  // @ro: 只读成员
    WriteOnly, // @wo: 只写成员
    ReadWrite, // @rw: 可读可写 (默认)
    ReadExec,  // @rx: 可读可执行
}
```

## 变量与内存枚举

### 变量修饰符 (VarModifier)

```rust
pub enum VarModifier {
    Var, // 完全可变 (隐含 mut)
    Val, // 逻辑不可变 (默认，允许内部可变性)
    Imv, // 物理不可变 (隐含 imm，存放在只读内存)
    Lit, // 编译期常量 (仅存在于编译期)
}
```

### 初始化策略 (InitStrategy)

```rust
pub enum InitStrategy {
    Normal, // 立即初始化
    Late,   // 手动延迟初始化 (late)
    Lazy,   // 线程安全懒加载 (lazy)
}
```

## 访问控制 (Access Control)

```rust
pub enum AccessModifier {
    Public,    // @public
    Protected, // @protected
    Private,   // @private
}
```

## 辅助结构

### 代码块 `Block`

```rust
pub struct Block {
    pub stats: Vec<Stat>,
    pub span: Span,
}
```

### 参数与捕获

```rust
pub struct Param {
    pub name: String,
    pub r#type: TypeRef,
    pub constraint: Option<Expr>, // where 子句
}

pub struct Capture {
    pub name: String,
    pub is_by_value: bool,
}
```

---

相关内容：AST 总览见 [语法树](index.md)。
