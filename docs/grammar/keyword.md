
# 关键字

## 1

| 关键字 | 全称                       | 用法                             |
|--------|----------------------------|----------------------------------|
| var    | variable                   | 声明完全可变变量（隐含 `mut`）   |
| val    | value                      | 声明逻辑不可变变量               |
| imv    | immutable variable         | 声明物理不可变变量（隐含 `imm`） |
| lit    | literal                    | 声明编译期常量                   |
| fin    | final variable             | 声明禁止重新绑定的变量           |
| let    | expression                 | 定义表达式                       |
| obj    | dynamically typed variable | 声明或定义动态类型变量           |

## 类型与访问修饰符

| 关键字 | 全称          | 用法               |
|--------|---------------|--------------------|
| mut    | mutable       | 类型修饰：完全可变 |
| imm    | immutable     | 类型修饰：物理不可变 |
| ro     | read-only     | 访问修饰：只读     |
| wo     | write-only    | 访问修饰：只写     |
| rw     | read-write    | 访问修饰：读写     |
| rx     | read-execute  | 访问修饰：读执行   |

> **注意**：`var` 隐含了 `mut`，`imv` 隐含了 `imm`。在声明变量时重复使用这些修饰符（如 `var mut`）会导致编译错误。

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
if (a is i32) {
    println("a is an integer");
}
```

`is` 关键字也可以用于判断一个对象是否处于某个状态。<br />
<span style="color:green">实现方法为使用对象中定义的 `is_状态名` 变量或函数。</span>

```lumos
if (a is empty) {
    println("a is empty");
}

if (a.is_empty) {
    println("a is empty");
}
```
