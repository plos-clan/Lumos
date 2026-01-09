
# 函数 {#functions}

## 函数分类与纯度 {#functions-class}

Lumos 根据副作用和纯度将函数分为三类，分别使用不同的关键字定义：

- `def`：**纯函数**。输入相同则输出必相同，无任何副作用。只能调用其他 `def` 函数。
- `fun`：**逻辑纯函数**。内部可以有局部副作用（如局部变量修改、内存分配），但对外部环境无影响。可以调用 `fun` 和 `def`。
- `act`：**副作用函数**。可以执行 IO、修改全局状态等。可以调用 `act`、`fun` 和 `def`。必须声明所需的[权限](permission.md)。

### 纯度检查示例 {#examples}

```lumos
def add(int a, int b) -> int = a + b;

fun calculate() {
    val x = add(1, 2); // OK: fun can call def
    // println("Result: {x}"); // Error: fun cannot call act `println`
}

act[io.out] main() {
    calculate(); // OK: act can call fun
    println("Done");
}
```

> **注意**：`act` 函数可以使用方括号 `[]` 声明所需的权限。如果不需要任何权限，直接使用 `act` 即可。

此外还有多个修饰符：

- `ovl`：**允许重载**。默认情况下函数不可重载。若要重载，该名称对应的**所有**定义都必须显式带有此关键字。
- `unsafe fun`：将有副作用的代码标记为逻辑纯，允许在 `fun` 中调用 `act`。
- `once? act` / `once! act`：承诺该动作只执行一次。`once?` 在再次调用时忽略，`once!` 在再次调用时报错。在第一次显式调用时执行。
  > **性能注意**：`once` 包含编译时和运行时的双重检查，应避免在性能敏感的场景（如高频循环）中使用。

## 调试伪函数 `dbg` {#functions-dbg}

为了方便调试而不破坏函数的纯度，Lumos 提供了一个特殊的伪函数 `dbg`：

- **全场景可用**：可以在 `def`、`fun`、`act` 等任何地方调用。
- **编译期输出**：如果在编译期求值（如 `lit` 初始化）中调用，编译器会将信息输出到 `stderr`。
- **运行时输出**：在程序运行时调用，会将信息输出到 `stderr`。
- **不破坏纯度**：调用 `dbg` 不会被视为副作用，因此不会导致 `def` 或 `fun` 降级为 `act`。

```lumos
def add(i32 a, i32 b) -> i32 {
    dbg(`adding $a and $b`); // 允许在纯函数中使用
    return a + b;
}
```

## 函数声明 {#functionsdeclaration}

```lumos
// 声明一个函数
[ovl] def 函数名[捕获列表](参数列表) -> 返回类型;
[ovl] fun 函数名[捕获列表](参数列表) -> 返回类型;
[ovl] act 函数名[捕获列表](参数列表) -> 返回类型;
```

- `ovl`：可选，标记该函数支持重载。
- 函数名：函数的标识符，用于在程序中调用该函数。
- 捕获列表：可选项，指定函数中使用的外部变量的捕获方式。
- 参数列表：可选项，包含函数的输入参数列表。
- 返回类型：可选项，指定函数返回值的类型。

## 函数定义 {#functionsdefinition}

```lumos
// 定义一个函数
def 函数名[捕获列表](参数列表) -> 返回类型 {函数体}
```

- 函数名、捕获列表、参数列表和返回类型的含义同函数声明。
- 函数体：包含了函数的实际实现代码。

## 单语句函数定义 {#functionsdefinition-2}

```lumos
// 定义一个单语句的函数
def 函数名[捕获列表](参数列表) = 表达式;
```

- 适用于只包含一条语句的函数，可以将函数体直接定义为一个表达式。
- 表达式的计算结果将作为函数的返回值。

## Lambda 表达式 {#lambda-expressions}

```lumos
// 定义一个 lambda 表达式
def [捕获列表](参数列表) -> 返回类型 {函数体}
fun [捕获列表](参数列表) -> 返回类型 {函数体}
act [捕获列表](参数列表) -> 返回类型 {函数体}
```

- Lambda 表达式同样需要指定纯度关键字。
- 不需要指定函数名，直接使用捕获列表、参数列表、返回类型和函数体定义 Lambda 函数。

## 返回类型推断 {#class}

- 当函数体中所有 `return` 处的类型一致时，可以省略返回类型。
- 若函数体中没有 `return` 语句，则返回类型推断为 `unit`。

## 不返回函数 {#functions-2}

对于永远不会返回的函数（如调用了 `exit` 或无限循环），使用 `-` 标记：

```lumos
act[io.out, sys.proc] fatal_error(string msg) - {
    println(msg);
    exit(1);
}
```

## 捕获列表 {#capture-list}

- 捕获列表用于指定函数中引用外部变量的方式。
- 可以省略捕获列表，默认为引用捕获所有外部变量。
- 支持引用捕获和值捕获两种方式。

## 参数列表 {#parameter-list}

- 参数列表用于指定函数的输入参数。
- 可以省略参数列表，表示函数没有参数。
- 当函数没有参数时，参数列表可以是 `()`，但不能像 C 语言那样写作 `(unit)`。

## 参数范围限定 `where` {#parameter-constraints-where}

我们用 `where` 关键字对函数参数进行范围限定：

```lumos
def sqrt(f32 x where x >= 0.0) -> f32 {
    // ...
}
```

`where` 后接一个布尔表达式，该表达式会在调用时进行检查，如果不满足则抛出异常。同时作为优化手段也会在编译期进行检查，如果能证明调用时该表达式总为真则不会生成运行时检查代码。

`where` 后的表达式必须为纯表达式，且只能使用参数变量和常量。

## 成员函数修饰符 {#functions-3}

在成员函数中，可以使用 `@` 符号标记函数对对象成员的访问权限：

- `@ro` (Read-Only)：只能读成员变量，不能写。
- `@wo` (Write-Only)：只能写成员变量，不能读。
- `@rw` (Read-Write)：可读可写成员变量（默认）。
- `@rx` (Read-Execute)：可读可执行成员变量，不能写。

```lumos
class MyClass {
    i32 value;

    def@ro get_value() -> i32 {
        return value; // 允许读
    }

    act[obj.write]@wo set_value(i32 v) -> unit {
        value = v; // 允许写
    }
}
```

## 示例 {#examples-2}

```lumos
def add(i32 a, i32 b) -> i32 {
  return a + b;
}

act[io.out] main() -> i32 {
  return add(1, -1);
}
```

## 异步函数 `async` {#functions-async}

Lumos 支持异步编程，通过 `async` 关键字修饰 `fun` 或 `act`：

- `async fun`：**异步逻辑纯函数**。用于执行耗时的计算任务，虽然涉及任务调度，但对外部逻辑保持透明，不产生 IO 或全局状态修改。
- `async act`：**异步副作用函数**。允许在异步执行过程中进行 IO 操作或修改全局状态。

注意：`def` 不支持异步，因为纯函数要求立即返回确定的结果。

```lumos
// 异步计算（逻辑纯）
async fun compute_pi(i32 precision) -> f64 {
    // 耗时计算
    return result;
}

// 异步 IO（副作用）
async act[net.http.client] fetch_url(string url) -> string {
    val content = await network.get(url);
    return content;
}
```

## 调用简写 {#call-shortcuts}

对于没有参数的函数，可以直接使用函数名来调用。

如果有以下函数：

```lumos
fun rand() -> i32 {
    @static
    var i32 seed = 1;
    seed = seed * 114514 + 1919810;
    return seed;
}
```

我们就可以：

```lumos
println(rand());
println(rand);
```

对于单个参数的函数，我们可以省略括号：

```lumos
println("Hello world!");
println "Hello world!";
```

## 规范 {#guidelines}

我们明确：一个函数只应该做一件事情，这样可以使代码更加清晰。

函数一般不超过 20 行语句，超过的话应该考虑拆分。

函数尽量不依赖全局状态，这样可以使函数更加独立，也便于进行单元测试。

函数的参数应该尽量少，大多数不超过 3 个，少数可以达到 6 个，超过的话应该考虑拆分。对于输入状态特别多的函数，应当使用结构体引用传参。

函数的参数应该尽量避免使用未进行限制的指针，除非有必要。

---

相关内容：权限系统见 [副作用权限系统](permission.md)。
