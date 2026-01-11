# 约束与断言 {#title}

演示 `where` 参数约束的基本用法。示例通过 `where x >= 0.0` 保证参数取值范围，违反条件时会在调用点触发检查。
> 相关语法： [参数范围限定 `where`](../grammar/function.md#where)。

说明：
`where` 表达式必须为纯表达式，只能引用参数与常量。编译器在能证明条件恒真时可省略运行时检查，从而获得优化收益。

```lumos: where_constraints.lm```
