
# 关键字

## 变量声明与初始化

| 关键字 | 全称 | 用法 |
|--------|------|------|
| var    | variable | 声明完全可变变量（隐含 `mut`） |
| val    | value | 声明逻辑不可变变量 |
| imv    | immutable variable | 声明物理不可变变量（隐含 `imm`） |
| lit    | literal | 声明编译期常量 |
| fin    | final variable | 声明禁止重新绑定的变量 |
| late   | late | 标记变量为手动延迟初始化 |
| lazy   | lazy | 标记变量为懒加载初始化 |
| let    | expression | 定义表达式 |
| obj    | dynamically typed variable | 声明或定义动态类型变量 |

## 函数与纯度

| 关键字 | 全称 | 用法 |
|--------|------|------|
| def    | define | 定义纯函数 |
| fun    | function | 定义逻辑纯函数 |
| act    | action | 定义副作用函数 |
| once?  | once (soft) | 修饰 `act`，再次调用时忽略 |
| once!  | once (strict) | 修饰 `act`，再次调用时报错 |
| unsafe | unsafe | 修饰 `fun`，允许在逻辑纯函数中执行副作用 |
| where  | where | 对函数参数或泛型进行范围限定 |

## 类型与访问修饰符

| 关键字 | 全称         | 用法                 |
|--------|--------------|----------------------|
| mut    | mutable      | 类型修饰：完全可变   |
| imm    | immutable    | 类型修饰：物理不可变 |
| ro     | read-only    | 访问修饰：只读       |
| wo     | write-only   | 访问修饰：只写       |
| rw     | read-write   | 访问修饰：读写       |
| rx     | read-execute | 访问修饰：读执行     |

> **注意**：`var` 隐含了 `mut`，`imv` 隐含了 `imm`。在声明变量时重复使用这些修饰符（如 `var mut`）会导致编译错误。

## 控制流程与逻辑

| 关键字 | 全称 | 用法 |
|--------|------|------|
| if     | if | 条件分支 |
| else   | else | 否则 |
| elif   | else if | 否则如果 |
| then   | then | 条件成立时执行 |
| switch | switch | 多分支选择 |
| match  | match | 模式匹配 |
| for    | for loop | 循环 |
| while  | while loop | 循环 |
| do     | do while loop | 循环 |
| break  | break | 跳出循环 |
| continue| continue | 跳过本次循环 |
| leave  | leave | 跳出当前块（如 switch） |
| goto   | goto | 跳转 |
| assume | assume | 静态假设条件成立 |

## 类型转换与判断

| 关键字 | 全称 | 用法 |
|--------|------|------|
| as     | as | 类型转换 |
| is     | is | 类型或状态判断 |

## 模块与结构

| 关键字 | 全称 | 用法 |
|--------|------|------|
| namespace | namespace | 命名空间 |
| struct | struct | 结构体 |
| class  | class | 类 |
| enum   | enum | 枚举 |
| union  | union | 联合体 |
| impl   | implement | 实现接口或方法 |
| using  | using | 导入模块或定义别名 |

### is

`is` 关键字用于判断一个值是否为某个类型。

```lumos
if (a is i32) {
    println("a is an integer");
}
```

`is` 关键字也可以用于判断一个对象是否处于某个状态。  
<span style="color:green">实现方法为使用对象中定义的 `is_状态名` 变量或函数。</span>

```lumos
if (a is empty) {
    println("a is empty");
}

if (a.is_empty) {
    println("a is empty");
}
```
