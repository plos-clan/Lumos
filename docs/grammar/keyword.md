
# 关键字

## 变量声明与初始化

| 关键字 | 全称                       | 用法                             | 参考                    |
|--------|----------------------------|----------------------------------|-------------------------|
| var    | variable                   | 声明完全可变变量（隐含 `mut`）   | [变量](variable.md)     |
| val    | value                      | 声明逻辑不可变变量               | [变量](variable.md)     |
| imv    | immutable variable         | 声明物理不可变变量（隐含 `imm`） | [变量](variable.md)     |
| lit    | literal                    | 声明编译期常量                   | [变量](variable.md)     |
| fin    | final variable             | 声明禁止重新绑定的变量           | [变量](variable.md)     |
| late   | late                       | 标记变量为手动延迟初始化         | [变量](variable.md)     |
| lazy   | lazy                       | 标记变量为懒加载初始化           | [变量](variable.md)     |
| let    | expression                 | 定义表达式                       | [表达式](expression.md) |
| obj    | dynamically typed variable | 声明或定义动态类型变量           | [对象](object.md)       |

## 函数与纯度

| 关键字 | 全称          | 用法                                     | 参考                |
|--------|---------------|------------------------------------------|---------------------|
| def    | define        | 定义纯函数                               | [函数](function.md) |
| fun    | function      | 定义逻辑纯函数                           | [函数](function.md) |
| act    | action        | 定义副作用函数                           | [函数](function.md) |
| once?  | once (soft)   | 修饰 `act`，再次调用时忽略               | [函数](function.md) |
| once!  | once (strict) | 修饰 `act`，再次调用时报错               | [函数](function.md) |
| unsafe | unsafe        | 修饰 `fun`，允许在逻辑纯函数中执行副作用 | [函数](function.md) |
| ovl    | overload      | 标记函数允许重载（所有重载变体均需标记） | [重载](overload.md) |
| where  | where         | 对函数参数或泛型进行范围限定             | [函数](function.md) |

## 类型与访问修饰符

| 关键字 | 全称         | 用法                 | 参考                |
|--------|--------------|----------------------|---------------------|
| mut    | mutable      | 类型修饰：完全可变   | [变量](variable.md) |
| imm    | immutable    | 类型修饰：物理不可变 | [变量](variable.md) |
| ro     | read-only    | 访问修饰：只读       | [变量](variable.md) |
| wo     | write-only   | 访问修饰：只写       | [变量](variable.md) |
| rw     | read-write   | 访问修饰：读写       | [变量](variable.md) |
| rx     | read-execute | 访问修饰：读执行     | [变量](variable.md) |

> **注意**：`var` 隐含了 `mut`，`imv` 隐含了 `imm`。在声明变量时重复使用这些修饰符（如 `var mut`）会导致编译错误。

## 控制流程与逻辑

| 关键字   | 全称          | 用法                    | 参考                     |
|----------|---------------|-------------------------|--------------------------|
| if       | if            | 条件分支                | [控制流](controlflow.md) |
| else     | else          | 否则                    | [控制流](controlflow.md) |
| elif     | else if       | 否则如果                | [控制流](controlflow.md) |
| then     | then          | 条件成立时执行          | [控制流](controlflow.md) |
| switch   | switch        | 多分支选择              | [控制流](controlflow.md) |
| match    | match         | 模式匹配                | [控制流](controlflow.md) |
| for      | for loop      | 循环                    | [控制流](controlflow.md) |
| while    | while loop    | 循环                    | [控制流](controlflow.md) |
| do       | do while loop | 循环                    | [控制流](controlflow.md) |
| break    | break         | 跳出循环                | [控制流](controlflow.md) |
| continue | continue      | 跳过本次循环            | [控制流](controlflow.md) |
| leave    | leave         | 跳出当前块（如 switch） | [控制流](controlflow.md) |
| goto     | goto          | 跳转                    | [控制流](controlflow.md) |
| assume   | assume        | 静态假设条件成立        | [控制流](controlflow.md) |

## 类型转换与判断

| 关键字 | 全称 | 用法           | 参考                    |
|--------|------|----------------|-------------------------|
| as     | as   | 类型转换       | [表达式](expression.md) |
| in     | in   | 单位换算       | [单位系统](measure.md)  |
| is     | is   | 类型或状态判断 | [关键字 is](#is)        |

## 模块与结构

| 关键字    | 全称      | 用法               | 参考                   |
|-----------|-----------|--------------------|------------------------|
| namespace | namespace | 命名空间           | [模块系统](modules.md) |
| struct    | struct    | 结构体             | [类型系统](type.md)    |
| class     | class     | 类                 | [类与对象](class.md)   |
| enum      | enum      | 枚举               | [枚举](enumerate.md)   |
| union     | union     | 联合体             | [类型系统](type.md)    |
| impl      | implement | 实现接口或方法     | [类与对象](class.md)   |
| use       | use       | 导入模块或定义别名 | [模块系统](modules.md) |
| measure   | measure   | 定义量纲           | [单位系统](measure.md) |
| unit      | unit      | 空类型             | [类型系统](type.md)    |

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
