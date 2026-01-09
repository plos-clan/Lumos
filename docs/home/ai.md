# AI 速览

此页用于快速定位项目结构与关键规范。

## 关键入口

- 项目定位与目标：`docs/overview.md`
- 语言参考入口：`docs/language.md`
- 词法：`docs/lexical/index.md`
- 语法：`docs/grammar/index.md`
- 权限系统：`docs/grammar/permission.md`
- 权限树：`docs/grammar/permissions.yaml`
- 标准库：`docs/std/index.md`
- 编译器实现：`docs/ast/index.md`、`docs/ir/index.md`

## 核心规则

- 函数纯度：`def`/`fun`/`act`（见 `docs/grammar/function.md`）。
- 权限声明：`act[...]` 必须显式列出副作用权限（见 `docs/grammar/permission.md`）。
- 变量不可变层级：`val`/`imv`/`lit`（见 `docs/grammar/variable.md`）。
- 模块导入关键字为 `use`（见 `docs/grammar/modules.md`）。

## 配置文件

- 仅允许存在一个配置文件：`lumos.yaml` / `lumos.json` / `lumos.toml`。  
- 三种格式共享同一配置树与键名（见 `docs/grammar/modules.md`）。

## 源码与标识符

- 源码编码仅允许 UTF-8/UTF-16/UTF-32，且禁止 BOM（见 `docs/lexical/index.md`）。
- 标识符默认仅 ASCII，Unicode 名称写作 `$"..."`（见 `docs/lexical/index.md`）。

## 语法要点速查

- 字符串转义固定长度：`\xNN`、`\uNNNN`（见 `docs/lexical/string.md`）。
- `\x` 插入 byte，`\u` 插入字符；普通字符串禁止 `\0`（字节串允许）。
- `use "mod" -> ns` 导入模块到命名空间（见 `docs/grammar/modules.md`）。
- `act[perm]` 权限树使用点号分层，支持组语法 `{}`（见 `docs/grammar/permission.md`）。

## 文档一致性约定

- 例子默认使用 `i32`/`u32` 等类型，不使用 `int/long`。
- 控制流与遍历语法以 `docs/grammar/controlflow.md` 为准。
- 标准库接口以 `docs/std/*.md` 为准，示例需与接口表一致。

## 关键词指引

- 关键字总表与跳转：`docs/grammar/keyword.md`
- 表达式与 `let`：`docs/grammar/expression.md`
- 控制流与循环：`docs/grammar/controlflow.md`

---

相关内容：阅读路线见 [阅读路线](guide.md)。
