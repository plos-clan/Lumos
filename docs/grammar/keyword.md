
# 关键字 {#title}

## 变量声明与初始化 {#declaration-init}

| 关键字   | 全称                       | 用法                                                             |
|----------|----------------------------|------------------------------------------------------------------|
| var      | variable                   | [声明完全可变变量（隐含 `mut`）](variable.md#keyword-var)        |
| val      | value                      | [声明逻辑不可变变量](variable.md#keyword-val)                    |
| imv      | immutable variable         | [声明物理不可变变量（隐含 `imm`）](variable.md#keyword-imv)      |
| ref      | reference                  | [声明引用变量](type.md#reference-types)                          |
| lit      | literal                    | [声明编译期常量](variable.md#keyword-lit)                        |
| fin      | final variable             | [声明禁止重新绑定的变量](variable.md#fin)                        |
| late     | late                       | [标记变量为手动延迟初始化](variable.md#late)                     |
| lazy     | lazy                       | [标记变量为懒加载初始化](variable.md#lazy)                       |
| volatile | volatile                   | [声明变量可被外部修改，禁止编译器缓存优化](variable.md#volatile) |
| restrict | restrict                   | [指针限定符](variable.md#restrict)                               |
| let      | expression                 | [定义表达式](expression.md#let)                                  |
| obj      | dynamically typed variable | [声明或定义动态类型变量](object.md#usage)                        |

<details>

<summary>关键字来源</summary>

- `var` 和 `val` 来自 Kotlin，分别表示可变和不可变变量。
- `late` 和 `lazy` 来自 Kotlin 的 `lateinit` 和 `by lazy`，分别表示手动延迟初始化和懒加载初始化。
- `restrict` 来自 C99 的 `restrict` 关键字，用于指针限定。

</details>

## 函数与纯度 {#functions-purity}

| 关键字   | 全称          | 用法                                                                              |
|----------|---------------|-----------------------------------------------------------------------------------|
| def      | define        | [定义纯函数](function.md#function-purity)                                         |
| fun      | function      | [定义逻辑纯函数](function.md#function-purity)                                     |
| obs      | observe       | [定义观测性函数（有副作用但不影响调用方计算结果）](function.md#function-purity)   |
| act      | action        | [定义副作用函数](function.md#function-purity)                                     |
| once?    | once (soft)   | [修饰 `act`，再次调用时忽略](function.md#function-purity)                         |
| once!    | once (strict) | [修饰 `act`，再次调用时报错](function.md#function-purity)                         |
| unsafe   | unsafe        | [修饰 `fun`，声明可回滚副作用（需权限列表与回滚块）](function.md#function-purity) |
| ovl      | overload      | [标记函数允许重载（所有重载变体均需标记）](overload.md#rules)                     |
| where    | where         | [对函数参数或泛型进行范围限定](function.md#where)                                 |
| rollback | rollback      | [在 `unsafe fun` 后声明回滚逻辑](function.md#rollback)                            |

<details>

<summary>关键字来源</summary>

- `def` 来自数学上的函数定义，强调纯函数的概念。
- `fun` 来自大多数编程语言使用的 "function" 一词，表示一般函数。
- `act` 来自 "action"，强调具有副作用的函数，由生成式 AI 推荐我使用。

> 实际上最早采用的是来自 Rust 的 `fn`，但后来由于分成了三类，就改成了现在的 `def`、`fun` 和 `act`。

</details>

## 异步与等待 {#async-await}

| 关键字 | 全称  | 用法                              |
|--------|-------|-----------------------------------|
| async  | async | [异步函数修饰](function.md#async) |
| await  | await | [异步等待](function.md#async)     |

## 权限与效应 {#permissions-effects}

| 关键字     | 全称       | 用法                                             |
|------------|------------|--------------------------------------------------|
| permission | permission | [定义权限](permission.md#permission-aliases)     |
| aka        | aka        | [权限别名](permission.md#permission-aliases)     |
| perm       | perm       | [声明上下文批量权限块](permission.md#perm-block) |
| yields     | yields     | [权限传播](permission.md#yielding)               |

## 类型与访问修饰符 {#types-access-modifiers}

| 关键字 | 全称         | 用法                                                          |
|--------|--------------|---------------------------------------------------------------|
| pub    | public       | [成员可见性：公共](class.md#access-control)                   |
| prv    | private      | [成员可见性：私有](class.md#access-control)                   |
| mut    | mutable      | [类型修饰：完全可变](variable.md#declaration)                 |
| imm    | immutable    | [类型修饰：物理不可变](variable.md#declaration)               |
| ro     | read-only    | [访问修饰：只读](variable.md#access-modifiers)                |
| wo     | write-only   | [访问修饰：只写](variable.md#access-modifiers)                |
| rw     | read-write   | [访问修饰：读写](variable.md#access-modifiers)                |
| rx     | read-execute | [访问修饰：读执行](variable.md#access-modifiers)              |

> **注意**：`var` 隐含了 `mut`，`imv` 隐含了 `imm`。在声明变量时重复使用这些修饰符（如 `var mut`）会导致编译错误。

## 类型与单位 {#types-measure}

| 关键字  | 全称    | 用法                                    |
|---------|---------|-----------------------------------------|
| unit    | unit    | [空类型](type.md#types)                 |
| measure | measure | [定义量纲](measure.md#dimensions-units) |

## 内置类型 {#builtin-types}

| 关键字                     | 用法                                            |
|----------------------------|-------------------------------------------------|
| bool                       | [布尔类型](type.md#types)                       |
| flag                       | [标志位类型](type.md#types)                     |
| byte                       | [字节类型](type.md#types)                       |
| char                       | [Unicode 字符](type.md#types)                   |
| ascii                      | [ASCII 字符](type.md#types)                     |
| char8/16/32                | [字符类型](type.md#types)                       |
| str                        | [字符串](type.md#string)                        |
| str8/16/32                 | [UTF 编码字符串](type.md#string)                |
| cstr                       | [C 字符串](type.md#string)                      |
| cstr8/16/32                | [UTF 编码 C 字符串](type.md#string)             |
| bytes                      | [字节数组](type.md#string)                      |
| int/uint                   | [编译期整数](type.md#compile-time-integer)      |
| float                      | [编译期浮点](type.md#compile-time-float)        |
| i8/16/32/64 **...**        | [整数类型](type.md#numeric-types)               |
| u8/16/32/64 **...**        | [无符号整数](type.md#numeric-types)             |
| f16/32/64/128              | [浮点类型](type.md#numeric-types)               |
| usize/isize                | [平台字长整数](type.md#numeric-types)           |
| b8/16/32/64 **...**        | [二进制类型](type.md#binary)                    |
| flag8/16/32/64 **...**     | [标志位类型](type.md#binary)                    |
| byte1/2/4/8 **...**        | [字节类型扩展](type.md#binary)                  |
| i**N**/u**N**/f**N**       | [位宽扩展类型](type.md#numeric-width-extension) |
| i**N**le/u**N**le/f**N**le | [小端类型](type.md#numeric-types)               |
| i**N**be/u**N**be/f**N**be | [大端类型](type.md#numeric-types)               |

## 字面量 {#literals}

| 关键字 | 全称        | 用法                                               |
|--------|-------------|----------------------------------------------------|
| true   | true        | [`bool` 真值字面量](type.md#types)                 |
| false  | false       | [`bool` 假值字面量](type.md#types)                 |
| on     | on (flag)   | [`flag` 置位字面量](type.md#types)                 |
| off    | off (flag)  | [`flag` 清位字面量](type.md#types)                 |

## 控制流程与逻辑 {#control-flow-logic}

| 关键字      | 全称          | 用法                                              |
|-------------|---------------|---------------------------------------------------|
| if          | if            | [条件分支](controlflow.md#if)                     |
| else        | else          | [否则](controlflow.md#else)                       |
| elif        | else if       | [否则如果](controlflow.md#elif)                   |
| then        | then          | [条件成立时执行](controlflow.md#then)             |
| switch      | switch        | [多分支选择](controlflow.md#switch)               |
| match       | match         | [模式匹配](controlflow.md#match)                  |
| for         | for loop      | [循环](controlflow.md#for)                        |
| while       | while loop    | [循环](controlflow.md#while)                      |
| do          | do while loop | [循环](controlflow.md#do-while)                   |
| break       | break         | [跳出循环](controlflow.md#break)                  |
| continue    | continue      | [跳过本次循环](controlflow.md#continue)           |
| leave       | leave         | [跳出当前块（如 switch）](controlflow.md#leave)   |
| fallthrough | fallthrough   | [跳到下一个 switch 条件块](controlflow.md#switch) |
| goto        | goto          | [跳转](controlflow.md#jump-to-label)              |
| return      | return        | [返回](controlflow.md#return)                     |
| broken      | broken        | [循环中断分支](controlflow.md#broken)             |

## 异常与错误处理 {#exceptions}

| 关键字 | 全称  | 用法                                       |
|--------|-------|--------------------------------------------|
| try    | try   | [尝试块](exception.md#handle-exceptions)   |
| catch  | catch | [捕获块](exception.md#handle-exceptions)   |
| or     | or    | [异常分支](exception.md#handle-exceptions) |
| throw  | throw | [抛出异常](exception.md#throw)             |

## 类型转换与判断 {#type-cast}

| 关键字 | 全称 | 用法                                                                               |
|--------|------|------------------------------------------------------------------------------------|
| as     | as   | [类型转换](type.md#type-cast)                                                      |
| in     | in   | [遍历循环（`for x in 容器`）](controlflow.md#loops-for)；[单位换算](measure.md#in) |
| is     | is   | [类型或状态判断](expression.md#is)                                                 |

## 断言与静态检查 {#assertions}

| 关键字 | 全称   | 用法                                      |
|--------|--------|-------------------------------------------|
| assert | assert | [断言检查](attribute.md#assertions)       |
| assume | assume | [静态假设条件成立](controlflow.md#assume) |

## 泛型与模板 {#generics-templates}

| 关键字   | 全称     | 用法                               |
|----------|----------|------------------------------------|
| template | template | [模板/泛型声明](template.md#title) |
| typename | typename | [类型参数](template.md#title)      |
| variant  | variant  | [代数类型](type.md#types)          |

## 类型类别 {#type-kinds}

| 关键字    | 全称      | 用法                          |
|-----------|-----------|-------------------------------|
| inttype   | inttype   | [整数类型类别](type.md#types) |
| floattype | floattype | [浮点类型类别](type.md#types) |
| numtype   | numtype   | [数字类型类别](type.md#types) |

## 语法辅助与保留 {#syntax-helpers}

| 关键字 | 全称  | 用法                                                                     |
|--------|-------|--------------------------------------------------------------------------|
| with   | with  | [临时变量](variable.md#with)；[捕获列表语法](function.md#capture-params) |
| from   | from  | [语法辅助/预留](index.md#syntax-samples)                                 |
| to     | to    | [语法辅助/预留](index.md#syntax-samples)                                 |
| using  | using | [语法辅助/预留](index.md#syntax-samples)                                 |
| of     | of    | [语法辅助/预留](index.md#syntax-samples)                                 |
| by     | by    | [枚举值类型约束](enumerate.md#types)                                     |

## 模块与结构 {#modules-structures}

| 关键字    | 全称      | 用法                                     |
|-----------|-----------|------------------------------------------|
| namespace | namespace | [命名空间](modules.md#import)            |
| use       | use       | [导入模块或定义别名](modules.md#import)  |
| struct    | struct    | [结构体](class.md#struct-as-class)       |
| class     | class     | [类](class.md#declaration)               |
| enum      | enum      | [枚举](enumerate.md#types)               |
| union     | union     | [联合体](type.md#types)                  |
| impl      | implement | [实现接口或方法](class.md#definition)    |
| initvar   | initvar   | [变量列表初始化钩子](class.md#list-init) |

---

相关内容：语法总览见 [语法](index.md#syntax-samples)。
