# 语句节点 `Stat`

## 空语句节点 `EmptyStat`

## if 语句节点 `IfStat`

- `Expr` 条件表达式
- `Block` 条件为真时执行的代码块
- `Block` 条件为假时执行的代码块

## while 语句节点 `WhileStat`

- `Expr` 条件表达式
- `Block` 循环体
- `bool` 是否是 do-while 循环

## for 语句节点 `ForStat`

- `Stat` 初始化语句
- `Expr` 条件表达式
- `Stat` 迭代语句
- `Block` 循环体

## break 语句节点 `BreakStat`

- `Sym` 循环标签

## continue 语句节点 `ContinueStat`
