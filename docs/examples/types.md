# 类型与结构 {#title}

展示结构体与类、变量列表初始化、成员函数调用。示例包含 `struct` 与 `class` 的对比用法，并通过 `${...}` 语法进行变量列表初始化。
> 相关语法： [类与结构体](../grammar/class.md#title)，[变量列表初始化](../grammar/class.md#list-init)，[访问控制](../grammar/class.md#access-control)。

说明：
`struct` 适合承载简单数据；`class` 适合包含行为与权限控制。示例中类使用 `@public` 显式开放字段，`inc()` 修改内部状态并展示成员函数调用方式。

```lumos: structs_classes.lm```
