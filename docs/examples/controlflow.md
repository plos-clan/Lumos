# 控制流 {#title}

包含条件分支、后置 `if`、`then` 以及多种循环形式。示例先用 `while` 与经典 `for` 展示不同循环结构，再用后置 `if` 与 `then` 体现表达式化控制流，最后用区间遍历展示范围循环。
> 相关语法： [控制流](../grammar/controlflow.md#title)，[`if` 分支](../grammar/controlflow.md#if)，[`then` 语句](../grammar/controlflow.md#then)，[遍历循环](../grammar/controlflow.md#loops-for)。

```lumos: controlflow.lm```

`switch`/`match` 示例：
展示基于值的 `switch` 与基于匹配的 `match`，二者都支持 `@default` 兜底分支，语法结构相近便于替换。
> 相关语法： [`switch`](../grammar/controlflow.md#switch)，[`match`](../grammar/controlflow.md#match)。

```lumos: switch_match.lm```
