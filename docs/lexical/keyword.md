
# 关键字 {#title}

关键字与保留字：

- 所有关键字都是保留字。
- 额外预留一些容易造成歧义的名称（如 `i0` `i999999` 等）为保留字。

## 变量声明与初始化 {#declaration-init}

| 关键字   | 全称                       | 用法                                                                        |
|----------|----------------------------|-----------------------------------------------------------------------------|
| var      | variable                   | [声明完全可变变量（隐含 `mut`）](../grammar/variable.md#keyword-var)        |
| val      | value                      | [声明逻辑不可变变量](../grammar/variable.md#keyword-val)                    |
| imv      | immutable variable         | [声明物理不可变变量（隐含 `imm`）](../grammar/variable.md#keyword-imv)      |
| ref      | reference                  | [声明引用变量](../grammar/type.md#reference-types)                          |
| lit      | literal                    | [声明编译期常量](../grammar/variable.md#keyword-lit)                        |
| fin      | final variable             | [声明禁止重新绑定的变量](../grammar/variable.md#keyword-fin)                |
| late     | late                       | [标记变量为手动延迟初始化](../grammar/variable.md#late)                     |
| lazy     | lazy                       | [标记变量为懒加载初始化](../grammar/variable.md#lazy)                       |
| volatile | volatile                   | [声明变量可被外部修改，禁止编译器缓存优化](../grammar/variable.md#volatile) |
| restrict | restrict                   | [指针限定符](../grammar/variable.md#restrict)                               |
| let      | expression                 | [定义表达式](../grammar/expression.md#let)                                  |
| obj      | dynamically typed variable | [声明或定义动态类型变量](../grammar/object.md#usage)                        |

??? Note "关键字来源"
    - `var` 和 `val` 来自 Kotlin，分别表示可变和不可变变量。
    - `late` 和 `lazy` 来自 Kotlin 的 `lateinit` 和 `by lazy`，分别表示手动延迟初始化和懒加载初始化。
    - `restrict` 来自 C99 的 `restrict` 关键字，用于指针限定。

## 函数与纯度 {#functions-purity}

| 关键字   | 全称          | 用法                                                                                         |
|----------|---------------|----------------------------------------------------------------------------------------------|
| def      | define        | [定义纯函数](../grammar/function.md#function-purity)                                         |
| fun      | function      | [定义逻辑纯函数](../grammar/function.md#function-purity)                                     |
| obs      | observe       | [定义观测性函数（有副作用但不影响调用方计算结果）](../grammar/function.md#function-purity)   |
| act      | action        | [定义副作用函数](../grammar/function.md#function-purity)                                     |
| once?    | once (soft)   | [修饰 `act`，再次调用时忽略](../grammar/function.md#function-purity)                         |
| once!    | once (strict) | [修饰 `act`，再次调用时报错](../grammar/function.md#function-purity)                         |
| effectful| effectful     | [修饰 `fun`，声明可回滚副作用（需权限列表与回滚块）](../grammar/function.md#function-purity) |
| ovl      | overload      | [标记函数允许重载（所有重载变体均需标记）](../grammar/overload.md#rules)                     |
| where    | where         | [对函数参数或泛型进行范围限定](../grammar/function.md#where)                                 |
| rollback | rollback      | [在 `effectful fun` 后声明回滚逻辑](../grammar/function.md#rollback)                            |

??? Note "关键字来源"
    - `def` 来自数学上的函数定义，强调纯函数的概念。
    - `fun` 来自大多数编程语言使用的 "function" 一词，表示一般函数。
    - `act` 来自 "action"，强调具有副作用的函数，由生成式 AI 推荐我使用。

    > 实际上最早采用的是来自 Rust 的 `fn`，但后来由于分成了三类，就改成了现在的 `def`、`fun` 和 `act`。

## 异步与等待 {#async-await}

| 关键字 | 全称  | 用法                                         |
|--------|-------|----------------------------------------------|
| async  | async | [异步函数修饰](../grammar/function.md#async) |
| await  | await | [异步等待](../grammar/function.md#async)     |

## 权限与效应 {#permissions-effects}

| 关键字     | 全称       | 用法                                                        |
|------------|------------|-------------------------------------------------------------|
| permission | permission | [定义权限](../grammar/permission.md#permission-aliases)     |
| aka        | aka        | [权限别名](../grammar/permission.md#permission-aliases)     |
| perm       | perm       | [声明上下文批量权限块](../grammar/permission.md#perm-block) |
| yields     | yields     | [权限传播](../grammar/permission.md#yielding)               |

## 类型与访问修饰符 {#types-access-modifiers}

| 关键字 | 全称         | 用法                                                        |
|--------|--------------|-------------------------------------------------------------|
| pub    | public       | [成员可见性：公共](../grammar/class.md#access-control)      |
| prv    | private      | [成员可见性：私有](../grammar/class.md#access-control)      |
| mut    | mutable      | [类型修饰：完全可变](../grammar/variable.md#declaration)    |
| imm    | immutable    | [类型修饰：物理不可变](../grammar/variable.md#declaration)  |
| ro     | read-only    | [访问修饰：只读](../grammar/variable.md#access-modifiers)   |
| wo     | write-only   | [访问修饰：只写](../grammar/variable.md#access-modifiers)   |
| rw     | read-write   | [访问修饰：读写](../grammar/variable.md#access-modifiers)   |
| rx     | read-execute | [访问修饰：读执行](../grammar/variable.md#access-modifiers) |

> **注意**：`var` 隐含了 `mut`，`imv` 隐含了 `imm`。在声明变量时重复使用这些修饰符（如 `var mut`）会导致编译错误。

## 类型与单位 {#types-measure}

| 关键字  | 全称    | 用法                                                   |
|---------|---------|--------------------------------------------------------|
| unit    | unit    | [空类型；同时也是其值字面量](../grammar/type.md#types) |
| measure | measure | [定义量纲](../grammar/measure.md#dimensions-units)     |

## 内置类型 {#builtin-types}

| 关键字                   | 用法                                                       |
|--------------------------|------------------------------------------------------------|
| bool                     | [布尔类型](../grammar/type.md#types)                       |
| flag                     | [标志位类型](../grammar/type.md#types)                     |
| byte                     | [字节类型](../grammar/type.md#types)                       |
| char                     | [Unicode 字符](../grammar/type.md#types)                   |
| ascii                    | [ASCII 字符](../grammar/type.md#types)                     |
| char8/16/32              | [字符类型](../grammar/type.md#types)                       |
| str                      | [字符串](../grammar/type.md#string)                        |
| str8/16/32               | [UTF 编码字符串](../grammar/type.md#string)                |
| cstr / cstr8/16/32       | [C 字符串 / UTF 编码 C 字符串](../grammar/type.md#string)  |
| bytes                    | [字节数组](../grammar/type.md#string)                      |
| int / uint               | [编译期整数](../grammar/type.md#compile-time-integer)      |
| float                    | [编译期浮点](../grammar/type.md#compile-time-float)        |
| i8/16/32/64 **...**      | [整数类型](../grammar/type.md#numeric-types)               |
| u8/16/32/64 **...**      | [无符号整数](../grammar/type.md#numeric-types)             |
| f16/32/64/128            | [浮点类型](../grammar/type.md#numeric-types)               |
| index / offset           | [索引与偏移类型](../grammar/type.md#index-offset-types)    |
| usize/isize              | [平台字长整数（兼容层）](../grammar/type.md#numeric-types) |
| b8/16/32/64 **...**      | [二进制类型](../grammar/type.md#binary)                    |
| flag8/16/32/64 **...**   | [标志位类型](../grammar/type.md#binary)                    |
| byte1/2/4/8 **...**      | [字节类型扩展](../grammar/type.md#binary)                  |
| i**N** / u**N** / f**N** | [位宽扩展类型](../grammar/type.md#numeric-width-extension) |
| **X**le / **X**be        | [小端类型 / 大端类型](../grammar/type.md#numeric-types)    |

> **注意**：`[iuf][0-9]+[a-z]*` 均为保留字，不能用作标识符。

??? Note "关键字来源"
    - `bool`、`char`、`str` 来自大多数编程语言的内置类型。
    - `flag` 来自常用的标志位类型，通常用于位掩码。
    - `cstr` 来自 C 语言的字符串类型。
    - `iN`、`uN`、`fN` 来自古早的位宽类型命名习惯。
    - `le` 和 `be` 后缀来自于小端和大端字节序的缩写。
    - `unit` 来自于函数式编程语言中的空类型概念，表示没有有意义的值。

## 字面量 {#literals}

| 关键字       | 用法                                                     |
|--------------|----------------------------------------------------------|
| unit         | [`unit` 类型的值字面量](../grammar/type.md#types)        |
| true / false | [`bool` 真值 / 假值 字面量](../grammar/type.md#types)    |
| on / off     | [`flag` 置位 / 清位 字面量](../grammar/type.md#types)    |
| null         | [可空指针的空值字面量](../grammar/type.md#pointer-types) |
| none         | [可选值的缺席字面量](../grammar/type.md#pointer-types)   |

## 控制流程与逻辑 {#control-flow-logic}

| 关键字      | 全称          | 用法                                                           |
|-------------|---------------|----------------------------------------------------------------|
| if          | if            | [条件分支](../grammar/controlflow.md#if)                       |
| else        | else          | [否则](../grammar/controlflow.md#else)                         |
| elif        | else if       | [否则如果](../grammar/controlflow.md#elif)                     |
| then        | then          | [条件成立时执行](../grammar/controlflow.md#then)               |
| switch      | switch        | [多分支选择](../grammar/controlflow.md#switch)                 |
| match       | match         | [模式匹配](../grammar/controlflow.md#match)                    |
| for         | for loop      | [循环](../grammar/controlflow.md#for)                          |
| while       | while loop    | [循环](../grammar/controlflow.md#while)                        |
| do          | do while loop | [循环](../grammar/controlflow.md#do-while)                     |
| break       | break         | [跳出循环](../grammar/controlflow.md#break)                    |
| continue    | continue      | [跳过本次循环](../grammar/controlflow.md#continue)             |
| leave       | leave         | [跳出当前块（如 switch）](../grammar/controlflow.md#leave)     |
| fallthrough | fallthrough   | [跳到下一个 switch 条件块](../grammar/controlflow.md#switch)   |
| goto        | goto          | [跳转](../grammar/controlflow.md#jump-to-label)                |
| return      | return        | [返回](../grammar/controlflow.md#return)                       |
| broken      | broken        | [循环中断分支](../grammar/controlflow.md#broken)               |
| completed   | completed     | [循环完成分支](../grammar/controlflow.md#after-exit-completed) |

## 异常与错误处理 {#exceptions}

| 关键字 | 全称  | 用法                                                  |
|--------|-------|-------------------------------------------------------|
| try    | try   | [尝试块](../grammar/exception.md#handle-exceptions)   |
| catch  | catch | [捕获块](../grammar/exception.md#handle-exceptions)   |
| or     | or    | [异常分支](../grammar/exception.md#handle-exceptions) |
| throw  | throw | [抛出异常](../grammar/exception.md#throw)             |

## 类型转换与判断 {#type-cast}

| 关键字 | 全称 | 用法                                                                                                     |
|--------|------|----------------------------------------------------------------------------------------------------------|
| as     | as   | [类型转换](../grammar/type.md#type-cast)                                                                 |
| in     | in   | [遍历循环（`for x in 容器`）](../grammar/controlflow.md#loops-for)；[单位换算](../grammar/measure.md#in) |
| is     | is   | [类型或状态判断](../grammar/expression.md#is)                                                            |

## 断言与静态检查 {#assertions}

| 关键字 | 全称   | 用法                                                 |
|--------|--------|------------------------------------------------------|
| assert | assert | [断言检查](../grammar/attribute.md#assertions)       |
| assume | assume | [静态假设条件成立](../grammar/controlflow.md#assume) |

## 泛型与模板 {#generics-templates}

| 关键字   | 全称     | 用法                                          |
|----------|----------|-----------------------------------------------|
| template | template | [模板/泛型声明](../grammar/template.md#title) |
| typename | typename | [类型参数](../grammar/template.md#title)      |
| variant  | variant  | [代数类型](../grammar/type.md#types)          |

## 类型类别 {#type-kinds}

| 关键字    | 全称      | 用法                                     |
|-----------|-----------|------------------------------------------|
| inttype   | inttype   | [整数类型类别](../grammar/type.md#types) |
| floattype | floattype | [浮点类型类别](../grammar/type.md#types) |
| numtype   | numtype   | [数字类型类别](../grammar/type.md#types) |

## 语法辅助与保留 {#syntax-helpers}

| 关键字 | 全称  | 用法                                                                                           |
|--------|-------|------------------------------------------------------------------------------------------------|
| with   | with  | [临时变量](../grammar/variable.md#with)；[捕获列表语法](../grammar/function.md#capture-params) |
| from   | from  | [语法辅助/预留](../grammar/index.md#syntax-samples)                                            |
| to     | to    | [语法辅助/预留](../grammar/index.md#syntax-samples)                                            |
| using  | using | [语法辅助/预留](../grammar/index.md#syntax-samples)                                            |
| of     | of    | [语法辅助/预留](../grammar/index.md#syntax-samples)                                            |
| by     | by    | [枚举值类型约束](../grammar/enumerate.md#types)                                                |

## 模块与结构 {#modules-structures}

| 关键字    | 全称      | 用法                                                |
|-----------|-----------|-----------------------------------------------------|
| namespace | namespace | [命名空间](../grammar/modules.md#import)            |
| use       | use       | [导入模块或定义别名](../grammar/modules.md#import)  |
| struct    | struct    | [结构体](../grammar/class.md#struct-as-class)       |
| class     | class     | [类](../grammar/class.md#declaration)               |
| enum      | enum      | [枚举](../grammar/enumerate.md#types)               |
| union     | union     | [联合体](../grammar/type.md#types)                  |
| impl      | implement | [实现接口或方法](../grammar/class.md#definition)    |
| initvar   | initvar   | [变量列表初始化钩子](../grammar/class.md#list-init) |

---

相关内容：语法总览见 [语法](../grammar/index.md#syntax-samples)。
