
# 关键字

## 1

| 关键字 | 全称                       | 用法                   |
|--------|----------------------------|------------------------|
| var    | variable                   | 声明或定义变量         |
| val    | immutable variable         | 声明或定义不可变变量   |
| let    | expression                 | 定义表达式             |
| lit    | constant expression        | 定义常量表达式         |
| obj    | dynamically typed variable | 声明或定义动态类型变量 |

## 流程控制

| 关键字 | 全称          | 用法 |
|--------|---------------|------|
| for    | for loop      |      |
| while  | while loop    |      |
| do     | do while loop |      |
| if     |               |      |
| else   |               |      |
| elif   |               |      |
| goto   |               |      |

## 2

| 关键字 | 全称 | 用法                         |
|--------|------|------------------------------|
| as     |      | 将左侧的值转换为右侧的类型   |
| is     |      | 判断一个值是否为某个类型     |
|        |      | 判断一个对象是否处于某个状态 |

## 3

| 关键字    | 全称 | 用法 |
|-----------|------|------|
| namespace |      |      |
| struct    |      |      |
| class     |      |      |
| enum      |      |      |
| union     |      |      |
| impl      |      |      |

### is

`is` 关键字用于判断一个值是否为某个类型。

```lumos
if (a is int) {
    println("a is an integer");
}
```

`is` 关键字也可以用于判断一个对象是否处于某个状态。<br>
<span style="color:green">实现方法为使用对象中定义的 `is_状态名` 变量或函数。</span>

```lumos
if (a is empty) {
    println("a is empty");
}

if (a.is_empty) {
    println("a is empty");
}
```
