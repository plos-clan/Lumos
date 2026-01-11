# 闭包与权限 {#title}

演示闭包纯度匹配与 `act` 权限继承。示例在 `def` 作用域内定义 `def` 闭包保证纯度一致，在 `act` 作用域内使用省略权限的 `act` 闭包以自动继承权限。
> 相关语法： [Lambda 表达式](../grammar/function.md#lambda)，[闭包与权限继承](../grammar/permission.md#closure-inheritance)。

说明：
`def` 环境下的闭包无法执行副作用；`act` 环境下的闭包若省略权限列表，则继承当前作用域权限集。这样既能保持纯度规则，又能减少权限声明的重复。

```lumos: closure_permissions.lm```
