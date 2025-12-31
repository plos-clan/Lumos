# 语句节点 `Stat`

## 空语句节点 `EmptyStat`

## if 语句节点 `IfStat`

```lumos
Expr cond      条件表达式
Stat if_body   条件为真时执行的代码
Stat else_body 条件为假时执行的代码
```

## while 语句节点 `WhileStat`

```lumos
Expr cond 条件表达式
Stat body 循环体
```

## do-while 语句节点 `DoWhileStat`

```lumos
Expr cond 条件表达式
Stat body 循环体
```

## for 语句节点 `ForStat`

```lumos
Stat initstat 初始化语句
Expr cond     条件表达式
Stat step     更新语句
Stat body     循环体
```

## break 语句节点 `BreakStat`

```lumos
Sym label 循环标签
```

## continue 语句节点 `ContinueStat`

```lumos
Sym label 循环标签
```

## 代码块节点 `Block`

```lumos
Array</Stat/> stats 语句列表
```
