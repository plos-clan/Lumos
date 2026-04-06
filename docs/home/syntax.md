# 语法速览 {#title}

此页用于快速掌握 Lumos 的语法与风格，能够直接写出正确示例。

## 关键入口 {#key-entries}

- 项目定位与目标：[概览](../overview.md)
- 语言参考入口：[语言参考](../language.md)
- 词法：[词法](../lexical/index.md)
- 语法：[语法](../grammar/index.md)
- 权限系统：[权限系统](../grammar/permission.md)
- 权限树：[权限清单](../grammar/permissions.md)
- 标准库：[标准库](../std/index.md)
- 编译器实现：[AST](../ast/index.md)、[IR](../ir/index.md)

## 核心规则 {#core-rules}

- 函数纯度：`def`/`fun`/`act`（见 [函数与纯度](../grammar/function.md)）。
- 权限声明：`act[...]` 必须显式列出副作用权限（见 [权限系统](../grammar/permission.md)）。
- `effectful fun` 必须写权限列表并提供尾随 `rollback`，仅 `effectful fun[]` 可被 `fun` 调用（见 [函数与纯度](../grammar/function.md)）。
- 变量不可变层级：`val`/`imv`/`lit`（见 [变量与初始化](../grammar/variable.md)）。
- 模块导入关键字为 `use`（见 [模块系统](../grammar/modules.md)）。

## 配置文件 {#config-file}

- 仅允许存在一个配置文件：`lumos.yaml` / `lumos.json` / `lumos.toml`。  
- 三种格式共享同一配置树与键名（见 [模块系统](../grammar/modules.md)）。

## 源码与标识符 {#identifiers}

- 源码编码仅允许 UTF-8/UTF-16/UTF-32，且禁止 BOM（见 [词法](../lexical/index.md)）。
- 标识符默认仅 ASCII，Unicode 名称写作 `$"..."`（见 [词法](../lexical/index.md)）。

## 语法要点速查 {#syntax-quicklook}

- 字符串转义固定长度：`\xNN`、`\uNNNN`（见 [字符串字面量](../lexical/string.md)）。
- `\x` 插入 byte，`\u` 插入字符；普通字符串禁止 `\0`（字节串允许）。
- `use "mod" -> ns` 导入模块到命名空间（见 [模块系统](../grammar/modules.md)）。
- `act[perm]` 权限树使用点号分层，支持组语法 `{}`（见 [权限系统](../grammar/permission.md)）。
- 数组用 `$[...]` 初始化，切片类型为 `[]T`，指针类型为 `[T]`（见 [数组](../grammar/array.md)、[类型系统](../grammar/type.md)）。
- `where` 用于参数约束，必须是纯表达式（见 [函数与纯度](../grammar/function.md)）。
- 后置 `if` 与 `then` 可写成表达式式控制流（见 [控制流](../grammar/controlflow.md)）。

## 文档一致性约定 {#doc-conventions}

- 例子默认使用 `i32`/`u32` 等类型，不使用 `int/long`。
- 控制流与遍历语法以 [控制流](../grammar/controlflow.md) 为准。
- 标准库接口以 [标准库目录](../std/index.md) 为准，示例需与接口表一致。

## 快速模板 {#quick-templates}

最小可运行程序：

```lumos
use "std" -> std;

act[io.out] main() -> i32 {
  std.println("Hello, Lumos!");
  return 0;
}
```

函数纯度与权限边界：

```lumos
def add(i32 a, i32 b) -> i32 = a + b;

fun compute(i32 x) -> i32 {
  return add(x, 1);
}

act[io.out] main() -> i32 {
  std.println(compute(41));
  return 0;
}
```

`effectful fun` 回滚语法（尾随 `rollback`，可访问函数体内变量）：

```lumos
effectful fun[] next_id() -> i32 {
  @static
  var i32 counter = 0;
  counter = counter + 1;
  return counter;
} rollback {
  counter = counter - 1;
}
```

闭包纯度与权限继承：

```lumos
def helper(i32 x) -> i32 {
  val add_one = def (i32 y) -> i32 { return y + 1; };
  return add_one(x);
}

act[io.out] main() -> i32 {
  val logger = act (msg: string) -> unit { std.println(msg); };
  logger("ok");
  return 0;
}
```

数组与切片：

```lumos
val arr = $[1, 2, 3, 4, 5];
[]i32 s = arr[1..3];
```

异常处理：

```lumos
act[exn] might_fail(i32 v) -> i32 { if (v < 0) throw "err"; return v; }

act[io.out] main() -> i32 {
  val v = might_fail(-1) or { return -1; };
  return 0;
}
```

## 关键词指引 {#keyword-guide}

- 关键字总表与跳转：[关键字](../lexical/keyword.md#title)
- 表达式与 `let`：[表达式](../grammar/expression.md)
- 控制流与循环：[控制流](../grammar/controlflow.md)

---

相关内容：阅读路线见 [阅读路线](guide.md)。
