# 函数 {#title}

Lumos 的函数基本上类似于其他现代编程语言中的函数，但它们具有更严格的纯度分类和权限系统，以确保代码的安全性和可维护性。

## 函数分类与纯度 {#function-purity}

Lumos 根据副作用和纯度将函数分为三类，分别使用不同的关键字定义：

- `def`：**纯函数**。无副作用，输入相同则输出必相同。只能调用其他 `def` 及 `obs`。
- `fun`：**逻辑纯函数**。相同输入对调用方产生相同的可观测结果。可有可回滚副作用（如内存分配，需配合 `rollback` 块）。可调用 `fun`、`def` 及 `obs`。
- `obs`：**观测性函数**。有副作用，但对调用方的计算结果无影响（返回类型必须为 `unit`）。可被 `def`、`fun`、`obs`、`act` 调用而无需调用方持有其所使用的权限。
- `act`：**副作用函数**。可执行 I/O、修改全局状态等。可调用所有函数。必须声明所需的[权限](permission.md#title)（方括号 `[]` 内）。

### 纯度检查示例 {#examples}

```lumos
def add(i32 a, i32 b) -> i32 = a + b;

obs[io.err] trace_op(string msg) {
    stderr.write(msg); // obs 函数可使用权限，但调用方无需持有
}

fun calculate() {
    val x = add(1, 2);      // OK: fun 可调用 def
    trace_op(`Result: $x`); // OK: fun 可调用 obs
    println(`Result: $x`);  // Error: fun 不能调用 act `println`
}

act[io.out] main() {
    calculate();     // OK: act 可调用 fun
    println("Done"); // OK: act 可调用 act
}
```

## 函数修饰符

- `ovl`：**允许重载**。默认情况下函数不可重载。若要重载，该名称对应的**所有**定义都必须显式带有此关键字。
- `unsafe fun`：将可回滚副作用标记为逻辑纯，允许在 `fun` 中使用，但必须满足以下约束：
  - 必须在函数体后紧接 `rollback` 块（语法详见[回滚块](#rollback)），回滚动作需并发安全。
  - 仅当权限列表为空时，才允许被 `fun` 调用；否则编译错误。
- `obs`：**观测性函数**。声明该函数虽有副作用（如写日志），但对调用方的计算结果无影响，可被 `def`、`fun`、`act` 在不消耗任何调用方权限的情况下调用。编译器强制以下约束：
  - 返回类型必须为 `unit`
  - 参数只能按值或不可变引用传递，不能修改调用方的可变数据
  - 函数内部抛出的异常不传播给调用方（静默忽略）
  - 函数自身的权限（如 `obs[io.err]`）由定义所在模块的配额满足，与调用方权限无关
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

!!! note "`dbg` 与 `obs` 的区别"
    `dbg` 是编译器内置的调试辅助，发布模式下完全消除，无运行时代价。  
    `obs` 是一种语言特性，允许**第三方库**将自己的副作用函数声明为"观测性"——在生产环境中仍可运行（如写入日志文件或监控系统），且可在任意纯度上下文中调用。  
    日志库通常使用 `obs` 实现，而不依赖 `dbg`。

## 函数定义 {#function-definition}

### 基本语法

```lumos
[ovl] def 函数名 with[捕获列表](参数列表) -> 返回类型 {函数体}
[ovl] fun 函数名 with[捕获列表](参数列表) -> 返回类型 {函数体}
[ovl] obs[权限列表] 函数名 with[捕获列表](参数列表) {函数体}
[ovl] act[权限列表] 函数名 with[捕获列表](参数列表) -> 返回类型 {函数体}
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

## 返回值重载与结果使用 {#return-overload}

为避免调用点因重载选择不同而改变行为，Lumos 对"仅返回类型不同"的重载采用严格限制。

### 返回值重载约束

同名、同参数列表（含泛型约束与 `where` 约束）仅允许出现以下一组重载：

- `-> T`
- `-> unit`

且两者必须满足以下条件：

- 都显式标记 `ovl`
- 纯度均为 `act`
- 权限列表一致
- 异常行为一致（可抛异常集合、`panic` 行为）
- 其他修饰一致（如 `async`、`once?`/`once!`）

### 重载选择规则（返回值相关）

- 在需要值的上下文（赋值、参数、`return`、表达式计算）中，优先选择 `-> T`
- 在语句位置（结果未被消费）中，若存在 `-> unit` 则选择 `-> unit`
- 若仍出现二义性，编译器拒绝推断并要求显式写法

### 未使用返回值错误

除 `unit` 外，任何表达式结果都必须被"使用"或"显式丢弃"，否则编译错误。

视为已使用：

- 参与赋值：`val x = f();`
- 作为参数：`g(f());`
- 直接返回：`return f();`
- 参与后续表达式：`f() + 1`

视为显式丢弃：

- `val _ = f();`

若语句位置调用返回 `T` 的函数且不存在 `-> unit` 重载，编译器报错并提示添加显式丢弃。

## 回滚块 `rollback` {#rollback}

`unsafe fun` 必须在函数体后紧接一个 `rollback` 块，声明当函数成功返回后、调用链后续抛出异常时如何撤销本次副作用。

```lumos
// 有返回值：rollback(绑定名) 将返回值绑定到指定名称
unsafe fun allocate(usize size) -> Ptr {
    return raw_malloc(size);
} rollback(ptr) {
    raw_free(ptr);   // ptr 绑定返回值；参数 size 同样在作用域内
}

// 无返回值：参数仍在作用域内
unsafe fun lock(Mutex m) {
    m.acquire();
} rollback {
    m.release();     // 参数 m 在作用域
}

// 多参数 + 返回值同时可用
unsafe fun connect(string host, u16 port) -> Socket {
    return sys_connect(host, port);
} rollback(sock) {
    sock.close();    // sock 绑定返回值，host/port 参数也在作用域
}
```

**触发时机**：当 `unsafe fun` 成功返回后，若调用链上后续代码抛出异常，各 `unsafe fun` 的 `rollback` 块按调用栈逆序依次执行。若 `unsafe fun` 自身内部执行失败（未执行到 `return`），回滚块不触发——因为副作用尚未生效。

**约束**：

- 回滚块必须并发安全
- 回滚块中不得使用 `throw` 或调用 `act[exn]` 函数
- 回滚块中可调用 `obs` 函数（如记录回滚日志）

## Lambda 表达式 {#lambda}

```lumos
def with[捕获列表](参数列表) -> 返回类型 {函数体}
fun with[捕获列表](参数列表) -> 返回类型 {函数体}
obs[权限列表] with[捕获列表](参数列表) {函数体}
act with[捕获列表](参数列表) -> 返回类型 {函数体}
```

Lambda 的纯度必须与所在作用域匹配。`obs` Lambda 可在任意作用域中定义，其权限由定义处的模块配额满足。如需高阶函数接受纯度低于自身的闭包，应将其声明为 `def[%]`、`fun[%]` 或 `act[%]`（见[效应多态](permission.md#polymorphism)）。

## 捕获与参数 {#capture-params}

**捕获列表** `with[...]`：指定外部变量的捕获方式

- 默认仅捕获不可变变量（`val`/`imv`/`lit`），为引用捕获
- 可变变量（`var`）需显式声明捕获方式（引用或值捕获）

**参数列表**：

- 可省略（无参数）或写 `()`
- 不能写作 `(unit)`

## 参数约束 `where` {#where}

使用 `where` 对参数进行范围限定。默认违反时触发 `panic`（调用方无需 `act[exn]`）：

```lumos
def sqrt(f32 x where x >= 0.0) -> f32 { /* ... */ }
```

若需要使违反约束时抛出**可捕获的异常**，可在函数上添加 `@where(exn)` 属性。此时函数必须在权限列表中声明 `exn`，调用方可以用 `or` 捕获 `ConstraintError`：

```lumos
@where(exn)
act[exn] safe_sqrt(f32 x where x >= 0.0) -> f32 { /* ... */ }

// 调用方捕获约束违反：
safe_sqrt(-1.0) or (e as ConstraintError) {
    println("invalid argument");
};
```

约束表达式必须为纯表达式，仅使用参数和常量。编译器会尽可能在编译期验证，避免生成运行时检查。若需要可恢复的错误但不希望使用异常，可改用 `-> T or E` 返回值模式。

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

`async act[权限列表]` 声明的权限适用于整个函数体，包括每个 `await` 挂起点后继续执行的部分。异步任务被调度到其他线程时，权限随任务传递，不依赖调用方的运行时上下文。闭包捕获的权限由**定义处**的权限上下文决定，而非执行处。

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
