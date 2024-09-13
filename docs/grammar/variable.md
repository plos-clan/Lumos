
# 变量与常量

## 变量声明 `variable` `value` `constant`

使用类似 C 的形式 `类型 变量名 = 表达式;` 声明并初始化一个变量。<br>
也可以使用类似 C++ 的类型推导 `var 变量名 = 表达式;` 进行声明并初始化。<br>
*注意不是 `auto`*

变量名可以是一个路径，比如 `my_namespace.my_variable`。
*这时 `my_namespace` 必须存在*

```lumos
var a = 1; // 这两个等效
int b = 1; // 这两个等效
namespace my_namespace; // 创建一个空的命名空间
var my_namespace.my_variable = 1;
```

### 2. 常量声明

```lumos
// 声明一个常量
const 类型 常量名 = 值;
```

- 使用 `const` 关键字声明常量。
- 类型：常量的数据类型，用于指定常量的数据类型。
- 常量名：常量的标识符，用于在程序中引用该常量。
- 值：常量的初始值，一旦赋值后不可更改。

### 3. 常量表达式声明

```lumos
// 声明一个常量表达式
val 类型 常量名 = 表达式;
```

- 使用 `val` 关键字声明常量表达式。
- 类型：常量的数据类型，用于指定常量的数据类型。
- 常量名：常量的标识符，用于在程序中引用该常量。
- 表达式：常量的初始值表达式，只能使用常量表达式。

### 4. 类型推断

- 允许省略变量和常量的类型声明，编译器会根据赋值表达式自动推断类型。
- 类型推断发生在变量名之前，符合你希望的类型前置规范。

### 5. 常量与指针

- 允许 `const` 关键字的单独使用，并支持类型推断。
- `const` 的写法类似于 C 语言，即类型前置。
- 允许 `int const* a` 这样的写法，但不允许 `int * const a` 这样的写法。
- 支持多变量声明，例如 `int* a, b;`，其中 `a` 和 `b` 都是指针类型。

### 6. 变量和常量作用域

- 变量和常量的作用域范围取决于其声明的位置。
- 变量的作用域通常为其声明所在的代码块，包括花括号 `{}` 内。
- 常量的作用域可以是全局范围或局部范围，取决于其声明的位置。

### 7. 命名规范

- 变量和常量的命名应具有描述性，易于理解。
- 使用驼峰命名法或下划线命名法命名变量和常量。