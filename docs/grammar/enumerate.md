
# 枚举 {#enum}

## 枚举作为类型 {#enum-as-type}

```lumos
enum 枚举类型名 by 值的类型 as type { // as type 可以省略
    枚举元素1 = 值1, // = 值1 可以省略，此时会按照默认值规则自动生成
    枚举元素2 = 值2, // 务必添加尾随逗号
    // ...
}
// 此时 typeof(枚举元素1) is 枚举类型名
```

- `值的类型` 可以是任何基本类型或用户自定义类型，此类型就是枚举实际存储时使用的类型。
- 若需要根据类型自动生成默认值，其必须实现 `\default` 和 `\next` 运算符。

也可以不写值的类型，此时默认为 `i32`。

```lumos
enum 枚举类型名 {
    枚举元素1 = 值1,
    枚举元素2 = 值2,
    // ...
}
```

如果省略值，则第一个枚举元素的值为类型的默认值，之后每个元素的默认值为上一个元素 `\next` 运算的结果。

```lumos
enum 枚举类型名 by i32 {
    枚举元素1, // 默认为 0
    枚举元素2, // 默认为 1
    枚举元素3, // 默认为 2
    // ...
}
```

由于 `string` 类型没有实现 `\next` 运算符，因此不能省略值。

### 将枚举元素转换成其值 {#enum-value-cast}

可以通过强制转换为其内部类型将枚举元素转换成其值，也可以用适用于 wrapper 的方法。

```lumos
val a = 枚举元素1 as 值的类型;
val b = 枚举元素1 as inner;
```

### 枚举表格 {#enum-table}

枚举表格允许将一个枚举类型与多个不同类型的值关联。这在需要对每个枚举变体维护多维数据（如多种表示形式、本地化字符串、HTTP 状态码和错误消息）时非常有用。

#### 基本语法 {#basic-syntax}

```lumos
enum 枚举类型名 by 值的类型 as table {
    [enum,      类型1, 类型2, ... , 类型N],
    [枚举元素1, 值1_1, 值1_2, ... , 值1_N],
    [枚举元素2, 值2_1, 值2_2, ... , 值2_N],
    // ...
}
```

**结构说明**：

- **第一行**：必须是 `[enum, ...]`，定义每列的类型。
- **后续行**：枚举元素及其对应的值，必须与第一行的类型一一对应。
- **类型一致性**：编译器会严格检查每列的类型，类型不匹配会导致编译错误。

#### 基本用法 {#basic-usage}

通过 `as 类型名` 将枚举元素转换成对应列的值：

```lumos
enum ReturnCode by i32 as table {
    [enum,     i32, string                 ],
    [OK,       200, "OK"                   ],
    [NotFound, 404, "Not Found"            ],
    [Error,    500, "Internal Server Error"],
}

println(ReturnCode::NotFound as i32);    // 输出 404
println(ReturnCode::NotFound as string); // 输出 "Not Found"
```

#### 复杂数据结构关联 {#complex-data-association}

表格列可以是任意类型，包括结构体、枚举、甚至函数指针：

```lumos
struct ErrorInfo {
    message: string,
    severity: i32,
}

enum HttpStatus by i32 as table {
    [enum,        i32, string,       ErrorInfo,
     act[io.out]],
    [OK,          200, "OK",         ${ "Success", 0 },
     act[io.out] { println("200 OK"); }],
    [BadRequest,  400, "Bad Request", ${ "Invalid input", 1 },
     act[io.out] { println("400 Bad Request"); }],
    [Unauthorized, 401, "Unauthorized", ${ "Auth required", 2 },
     act[io.out] { println("401 Unauthorized"); }],
}

act[io.out] main() {
    val status = HttpStatus::BadRequest;
    val code = status as i32;           // 400
    val msg = status as string;         // "Bad Request"
    val info = status as ErrorInfo;     // { "Invalid input", 1 }
    val handler = status as act[io.out];// 对应的日志处理函数
    handler();                          // 执行处理函数
}
```

#### 边界与错误处理 {#bounds-and-errors}

**缺失值的处理**：

如果枚举元素在表格中缺失某列的值，编译器会报错。必须为表格中的每个枚举元素的每一列提供值：

```lumos
enum Status by i32 as table {
    [enum,    i32, string],
    [Running, 1,   "Running"],
    [Stopped, 2],  // 编译错误：缺少 string 列的值
}
```

**类型转换失败**：

如果尝试转换为未在表格中定义的列类型，编译器会报错：

```lumos
enum Color by i32 as table {
    [enum,  i32, string],
    [Red,   255, "Red"],
    [Green, 0,   "Green"],
}

val hex = Color::Red as f64; // 编译错误：f64 不在表格的列类型中
```

#### 与枚举内函数结合 {#enum-methods}

枚举表格与枚举方法可以结合使用，方法可以访问当前枚举元素的所有表格值：

```lumos
enum Operation by i32 as table {
    [enum,   i32, string, (i32, i32)->i32],
    [Add,    1,   "加法", def (a, b) -> i32 { return a + b; }],
    [Sub,    2,   "减法", def (a, b) -> i32 { return a - b; }],
    [Mul,    3,   "乘法", def (a, b) -> i32 { return a * b; }];

    fun describe() -> string {
        val op_name = this as string;  // 获取当前元素的名称
        return `Operation: $op_name`;
    }

    fun apply(i32 a, i32 b) -> i32 {
        val f = this as (i32, i32)->i32;  // 获取对应的函数
        return f(a, b);
    }
}

act[io.out] main() {
    println(Operation::Add.describe());  // 输出 "Operation: 加法"
    println(Operation::Add.apply(3, 5)); // 输出 8
}
```

#### 性能特性 {#performance-notes}

- **零运行时开销**：表格转换在编译期完全展开，无运行时查表操作。
- **内存布局**：通过 `as 类型` 的转换不会产生额外的内存占用，直接返回对应的值。
- **类型安全**：所有表格访问都在编译期类型检查，不存在无效转换的风险。

## 枚举作为容器 {#enum-as-container}

也可以将枚举用作一个容器，其中的元素可以直接参与运算。

```lumos
enum 枚举名 by 值的类型 as container {
    枚举元素1 = 值1,
    枚举元素2 = 值2,
    // ...
}
// 此时 typeof(枚举元素1) is 值的类型
```

注意此时枚举本身并**不是一个类型**，以下代码会导致报错：

```lumos
枚举名 my_var = 枚举元素1; // error: `枚举名` 不是一个类型
```

你也可以认为这是一个只能存放特定类型值的命名空间。

## 限制枚举只在编译期有效 {#compile-time-only-enum}

将值的类型设为 `unit` 可以阻止枚举元素的值在运行时被访问，此时枚举元素只能用于编译期的判断，在任何地方存储这个值会导致编译错误。

```lumos
enum 枚举名 by unit {
    枚举元素1,
    枚举元素2,
    // ...
}
```

## 枚举内函数 {#enum-functions}

枚举作为类型时内部可以定义函数。  
但作为容器时只能定义静态函数。

```lumos
enum 枚举名 by 值的类型 as type {
    枚举元素1,
    枚举元素2; // 以分号结尾

    fun 函数名(参数列表) -> 返回类型 {
        // 函数体
    }
    // ...
}
```

```lumos
enum 枚举名 by 值的类型 as container {
    static fun 函数名(参数列表) -> 返回类型 {
        // 函数体
    }
    // ...
}
```

---

相关内容：类型系统见 [类型系统](type.md#types)。
