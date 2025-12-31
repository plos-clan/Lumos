# Lumos AST 定义 (Rust 描述)

本目录使用 Rust 语言完整定义了 Lumos 的抽象语法树 (AST) 结构。Rust 的枚举 (Enum) 和模式匹配特性非常适合描述复杂的语法结构。

为了便于查阅，AST 定义被拆分为以下几个部分：

## 1. 目录索引

- **[基础元数据与枚举 (Common)](common.md)**
  - 包含源代码位置 (`Span`, `SourceLocation`)、函数纯度、各类修饰符（访问控制、变量修饰、初始化策略）以及 `Block`、`Param` 等基础结构。
- **[类型系统 (Type)](type.md)**
  - 定义了 `TypeRef` 结构，涵盖基础类型、复合类型（指针、引用、数组）、泛型、计量单位类型及效应多态。
- **[声明 (Declaration)](decl.md)**
  - 定义了顶层声明 `Decl`，包括函数 (`FunctionDecl`)、变量 (`VariableDecl`)、类 (`ClassDecl`)、结构体、枚举、模块以及权限别名。
- **[语句 (Statement)](stat.md)**
  - 定义了 `Stat` 结构，涵盖表达式语句、代码块、条件分支、循环、跳转、权限操作块、异常处理 (`Try`/`Catch`) 等。
- **[表达式 (Expression)](expr.md)**
  - 定义了 `Expr` 结构，涵盖字面量引用、运算（二元/一元/三元）、调用、成员访问、Lambda、类型转换 (`Cast`)、类型判断 (`Is`)、异步等待 (`Await`) 等。
- **[字面量 (Literal)](literal.md)**
  - 定义了 `Literal` 结构，涵盖数值、字符串、布尔、字符、单位数值、数组及对象初始化列表。

---

## 2. 设计原则

1. **显式性**：AST 结构反映了 Lumos 对副作用和内存安全的显式控制（如 `Purity` 和 `VarModifier`）。
2. **强类型**：通过 `TypeRef` 确保类型信息在语法树阶段即被充分捕获。
3. **可扩展性**：使用 Rust 的 `enum` 确保未来增加新语法特性（如新的效应或运算符）时具有良好的向后兼容性。
