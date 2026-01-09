# Lumos AST 定义 (Rust 描述)

本目录使用 Rust 语言完整定义了 Lumos 的抽象语法树 (AST) 结构。Rust 的枚举 (Enum) 和模式匹配特性非常适合描述复杂的语法结构。

为了便于查阅，AST 定义被拆分为以下几个部分：

## 目录索引

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

## 设计原则

1. **显式性**：AST 结构反映了 Lumos 对副作用和内存安全的显式控制（如 `Purity` 和 `VarModifier`）。
2. **强类型**：通过 `TypeRef` 确保类型信息在语法树阶段即被充分捕获。
3. **可扩展性**：使用 Rust 的 `enum` 确保未来增加新语法特性（如新的效应或运算符）时具有良好的向后兼容性。

## AST 节点命名树

Lumos 源码会被解析为一个 AST 树，其中 AST 节点包含其它节点和 Token 或 Token Tree。

AST 中每个元素都会有自己的 Span 和 ElementType 信息，以便进行定位。

AST 结构也被用作语义高亮和代码分析的基础。

为了便于理解和查找，下面列出了主要 AST 节点的命名树结构：

- `txt`: 纯文本节点
- `err`: 错误节点
  - `err.lex`: 词法错误
  - `err.par`: 语法错误
- `warn`: 警告节点
- `info`: 信息节点
- `ws`: 空白节点
  - `ws.sp`: 空格、制表符等空白字符
  - `ws.nl`: 换行
  - `ws.comment`: 注释节点
- `kw`: 关键字枚举
  - 其中 `kw.*` 定义了所有关键字的标识符
- `op`: 操作符枚举
  - 其中 `op.*` 定义了所有操作符的标识符
- `id`: 标识符节点
  - `id.var`: 变量标识符
  - `id.func`: 函数标识符
  - `id.type`: 类型标识符
- `lit`: 字面量节点
  - `lit.num`: 数值字面量
  - `lit.str`: 字符串字面量
    - `.open`: 字符串开引号
    - `.close`: 字符串闭引号
    - `.content`: 字符串内容
    - `.text`: 普通文本
    - `.esc`: 转义序列
      - `.bz`: 空字符转义
      - `.lf`: 换行转义
      - `.cr`: 回车转义
      - `.tab`: 制表符转义
      - `.del`: 删除符转义
      - `.x`: 十六进制转义
      - `.u`: Unicode 转义
      - `.cp`: Unicode 码点转义
  - `lit.char`: 字符字面量
    - `.open`: 字符开引号
    - `.close`: 字符闭引号
    - `.content`: 字符内容
  - `lit.bool`: 布尔字面量
  - `lit.unit`: 无量字面量
- `expr`: 表达式节点
  - `expr.prefix`: 前缀表达式
  - `expr.postfix`: 后缀表达式
  - `expr.infix`: 中缀表达式
  - `expr.call`: 函数调用表达式
  - `expr.member`: 成员访问表达式
- `punc`: 标点符号节点
  - `punc.semicolon`: 分号
  - `punc.comma`: 逗号
  - `punc.dot`: 点号
  - `punc.colon`: 冒号
  - `punc.double_colon`: 双冒号
  - `punc.arrow`: 箭头符号

### 语义高亮规则

我们使用类似 CSS 选择器的语法来定义语义高亮规则：

```css
kw {
  color: blue;
}
```

表示所有关键字节点将被高亮为蓝色。

```css
id.var {
  color: green;
}
```

表示所有变量标识符节点将被高亮为绿色。

---

相关内容：语法入口见 [语法](../grammar/index.md)。
