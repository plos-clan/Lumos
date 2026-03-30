# 函数 {#title}

Lumos 的函数基本上类似于其他现代编程语言中的函数，但它们具有更严格的纯度分类和权限系统，以确保代码的安全性和可维护性。

## 函数分类与纯度 {#function-purity}

Lumos 根据副作用和纯度将函数分为三类，分别使用不同的关键字定义：

- `def`：**纯函数**。无副作用，输入相同则输出必相同。只能调用其他 `def`。
- `fun`：**逻辑纯函数**。可有可回滚副作用（如内存分配）或不影响逻辑的副作用（如日志）。可调用 `fun` 和 `def`。
- `act`：**副作用函数**。可执行 I/O、修改全局状态等。可调用所有函数。必须声明所需的[权限](permission.md#title)（方括号 `[]` 内）。

### 纯度检查示例 {#examples}

```lumos
def add(i32 a, i32 b) -> i32 = a + b;

fun calculate() {
    val x = add(1, 2);     // OK: fun can call def
    println(`Result: $x`); // Error: fun cannot call act `println`
}

act[io.out] main() {
    calculate();     // OK: act can call fun
    println("Done"); // OK: act can call act
}
```

## 函数修饰符

- `ovl`：**允许重载**。默认情况下函数不可重载。若要重载，该名称对应的**所有**定义都必须显式带有此关键字。
- `unsafe fun`：将可回滚副作用标记为逻辑纯，允许在 `fun` 中使用，但必须满足以下约束：
  - 必须提供回滚块（在定义处强制），回滚动作需并发安全。
  - 仅当权限列表为空时，才允许被 `fun` 调用；否则编译错误。
- `once? act` / `once! act`：承诺该动作只执行一次。`once?` 在再次调用时忽略，`once!` 在再次调用时报错。在第一次显式调用时执行。
  > **性能注意**：`once` 包含编译期和运行时的双重检查，应避免在性能敏感的场景（如高频循环）中使用。

## 调试函数 `dbg` {#dbg}

Lumos 提供特殊伪函数 `dbg` 用于调试，不破坏函数纯度：

- 可在任何函数中调用，不视为副作用
- 编译期和运行时均输出到 `stderr`
- 发布模式下完全移除，无开销

```lumos
def add(i32 a, i32 b) -> i32 {
    dbg(`adding $a and $b`);
    return a + b;
}
```

## 函数定义 {#function-definition}

### 基本语法

```lumos
[ovl] def 函数名 with[捕获列表](参数列表) -> 返回类型 {函数体}
[ovl] fun 函数名 with[捕获列表](参数列表) -> 返回类型 {函数体}
[ovl] act 函数名 with[捕获列表](参数列表) -> 返回类型 {函数体}
```

- `[ovl]`：可选，允许函数重载（该名称的所有定义都需此标记）
- `with[捕获列表]`：可选，指定外部变量的捕获方式
- `(参数列表)`：可选，无参时可省略或写 `()`
- `-> 返回类型`：可选，不返回值时推断为 `unit`

### 单语句定义

```lumos
def 函数名(参数列表) = 表达式;
```

### 不返回函数

使用 `-` 标记永不返回的函数（调用 `exit` 或无限循环）：

```lumos
act[io.out, sys.proc] fatal_error(string msg) - {
    println(msg);
    exit(1);
}
```

### 返回类型推断

- 所有 `return` 类型一致时可省略返回类型
- 无 `return` 语句时推断为 `unit`

## Lambda 表达式 {#lambda}

```lumos
def with[捕获列表](参数列表) -> 返回类型 {函数体}
fun with[捕获列表](参数列表) -> 返回类型 {函数体}
act with[捕获列表](参数列表) -> 返回类型 {函数体}
```

Lambda 的纯度必须与所在作用域匹配。例外：`def`/`fun` 可接受 `act` 闭包参数，调用时若在 `act` 上下文中自动降级。

## 捕获与参数 {#capture-params}

**捕获列表** `with[...]`：指定外部变量的捕获方式

- 默认仅捕获不可变变量（`val`/`imv`/`lit`），为引用捕获
- 可变变量（`var`）需显式声明捕获方式（引用或值捕获）

**参数列表**：

- 可省略（无参数）或写 `()`
- 不能写作 `(unit)`

## 参数约束 `where` {#where}

使用 `where` 对参数进行范围限定，违反时抛出异常：

```lumos
def sqrt(f32 x where x >= 0.0) -> f32 { /* ... */ }
```

约束表达式必须为纯表达式，仅使用参数和常量。编译器会尽可能在编译期验证，避免生成运行时检查。

## 成员函数修饰符 {#member-modifiers}

在成员函数中，可以使用 `@` 符号标记函数对对象成员的访问权限：

- `@ro` (Read-Only)：只能读成员变量，不能写。
- `@wo` (Write-Only)：只能写成员变量，不能读。
- `@rw` (Read-Write)：可读可写成员变量（默认）。
- `@rx` (Read-Execute)：可读可执行成员变量，不能写。

```lumos
class MyClass {
    i32 value;

    def@ro get_value() -> i32;

    act@wo set_value(i32 v) -> unit;
}

impl MyClass {
    def@ro get_value() -> i32 {
        return value; // 允许读
    }

    act@wo set_value(i32 v) -> unit {
        value = v; // 允许写
    }
}
```

## 异步函数 {#async}

通过 `async` 修饰 `fun` 或 `act`（`def` 不支持异步）：

- `async fun`：异步逻辑纯函数，用于耗时计算
- `async act`：异步副作用函数，允许异步 I/O 操作

```lumos
async fun compute_pi(i32 precision) -> f64 { /* ... */ }
async act[net.http.client] fetch_url(string url) -> string {
    val content = await network.get(url);
    return content;
}
```

## 调用简写 {#write}

- 无参数函数可省略括号：`rand()` → `rand`
- 单参数函数可省略括号：`println("text")` → `println "text"`

## 编码指南 {#guidelines}

- 单一职责：一个函数只做一件事
- 长度：不超过 20 行，过长则拆分
- 无全局依赖：便于测试和维护
- 参数数量：通常不超过 3 个，最多 6 个；过多用结构体传参
- 避免无边界指针参数，除非必要
- 接口设计：使用 `fun` 而非 `def`，以方便隐藏必要的副作用和内存分配

---

参见：[副作用权限系统](permission.md#title)
