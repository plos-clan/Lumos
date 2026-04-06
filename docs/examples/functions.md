# 函数与纯度 {#title}

演示 `def`/`fun`/`act` 的分层，以及 `effectful fun` 的回滚语法。第一个示例强调纯度分级的调用边界：`def` 只能调用 `def`，`fun` 可调用 `def`/`fun`，`act` 具备副作用权限。第二个示例展示 `effectful fun` 必须带权限列表与尾随 `rollback`，并说明回滚用于撤销可追踪副作用。
> 相关语法： [函数与纯度](../grammar/function.md#function-purity)，[函数定义](../grammar/function.md#function-definition)，[副作用权限系统](../grammar/permission.md#title)。

说明：
第一个示例中 `compute` 作为 `fun` 只使用纯函数 `add`，保持逻辑纯度；`main` 使用 `act[io.out]` 输出。第二个示例中 `rollback` 直接访问函数体内变量，语义等同于在 `return` 后执行，且要求多 `return` 情况下只使用所有分支都可达的变量。

```lumos: purity_permissions.lm```

```lumos: effectful_fun_rollback.lm```
