
# 控制语句

- `#end` 用于结束一个块，块可以由 `#if` `#while` 等开启。

## 分支

- `#if`
- `#elif`
- `#else`

### 特殊分支

- `#ifdebug` 如果是调试模式则执行
- `#ifndebug` 如果不是调试模式则执行

## 循环

- `#while`<br>
  `#while 1 <= 10`
- `#for`<br>
  `#for i from 1 to 10`

## 跳转

> 我嘞个 goto 地狱啊

- `#.L1` 建立一个 label
- `#goto L1` 跳转到 label L1

## 示例
