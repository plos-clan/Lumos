# 副作用权限系统 (Side-Effect Permission System) {#title}

Lumos 采用静态效应系统，将副作用视为一种受控的“能力 (Capability)”。

## 核心原则 {#core-principles}

- **零信任**：`act main` 默认没有任何权限。
- **静态检查**：所有权限在编译期校验，无运行时开销。
- **显式授权**：副作用必须在 `act` 函数签名或代码块上显式标注。
- **纯度隔离**：`def` 和 `fun` 的权限集恒为空（仅内置 `dbg`，以及可调用 `obs`）；`unsafe fun` 需显式权限列表；`act` 可以声明权限。
- **观测隔离**：`obs` 函数的权限由定义所在模块的配额满足，调用方无需持有，也不参与调用方的权限集传播。

## 权限层级 {#permission-hierarchy}

权限采用点号分隔的树状结构。完整的权限树定义请参考 [权限清单](permissions.md#title)。

### 常用内置权限 {#built-in-permissions}

| 权限路径          | 别名 (aka) | 描述                               |
|-------------------|------------|------------------------------------|
| `io.out`          | `stdout`   | 标准输出流                         |
| `io.err`          | `stderr`   | 标准错误流                         |
| `io.in`           | `stdin`    | 标准输入流                         |
| `fs.read`         | -          | 文件读取                           |
| `fs.write`        | -          | 文件写入                           |
| `net.http.client` | -          | HTTP 客户端请求                    |
| `sys.env`         | -          | 访问环境变量                       |
| `exn`             | -          | 允许异常穿越函数边界（栈展开传播） |

> **提示**：别名（如 `stdout`）在代码中与完整路径（如 `io.out`）完全等价。

### 权限定义与别名 {#permission-aliases}

库或模块可以定义自己的权限，并使用 `aka` 关键字提供简写或兼容性名称：

```lumos
permission my_lib.network.socket aka socket;
```

- 拥有父权限等价于拥有其下所有子权限。

支持组语法：
`act[net.http{client, server}]` 等价于 `act[net.http.client, net.http.server]`。

## 权限操作 {#permission-ops}

权限控制是代码块级别的，子块默认继承父块权限。

- **`act[+xxx];` (提升当前块)**：在当前代码块后续部分增加权限。
- **`act[+xxx] { ... }` (提升子块)**：仅在指定的子块中增加权限。
- **`act[-xxx]` (屏蔽当前块)**：在当前块及其子块中屏蔽特定权限。
- **`act[-xxx] { ... }` (屏蔽子块)**：仅在指定的子块中屏蔽特定权限。
- **`act[xxx]` (重置当前块)**：当前块仅保留指定的权限，其余继承来的权限全部丢弃。
- **`act[xxx] { ... }` (重置子块)**：仅在指定的子块中重置权限。
- **`act[*, "reason"]` (逃生舱)**：强制获得所有权限。要求提供字符串理由，便于审计。
- **`act[*, "reason"] { ... }` (子块逃生舱)**：仅在指定子块中强制获得所有权限。要求提供字符串理由，便于审计。
- **`act[?]`**：编译器指令。在编译期打印当前代码块所拥有的完整权限树。

## 函数权限 {#function-permissions}

### 权限声明 {#declaration}

```lumos
act[net.http.client] fetch_data(string url) -> string { ... }
```

如果函数无需任何权限，直接使用 `act` 关键字即可：

```lumos
act main() { ... }
```

`obs` 函数的权限声明与 `act` 语法相同，但其权限由定义所在模块的配额满足，调用方无需持有这些权限：

```lumos
obs[io.err] log_info(string msg) { ... }  // 调用方不需要 io.err
```

### 批量权限 `perm` 块 {#perm-block}

在**声明上下文**（顶层、命名空间体、类体）中，可以用 `perm[权限]` 块为其中所有函数定义统一附加权限，避免在每个函数签名上重复书写。

```lumos
perm[exn] {
    act open_file(string path) -> File { ... }   // 等效于 act[exn]
    act read_file(File f) -> string { ... }      // 等效于 act[exn]
    act close_file(File f) { ... }               // 等效于 act[exn]
}
```

`perm` 块的权限与函数自身声明的权限**合并（取并集）**，不会覆盖：

```lumos
perm[exn] {
    act[io.out] print_header() { ... }   // 等效于 act[exn, io.out]
    act[fs.write] write_data() { ... }   // 等效于 act[exn, fs.write]
}
```

支持 `+`（添加）和 `-`（移除）修饰符：

```lumos
perm[+exn, +io.err] {
    act process() { ... }   // 等效于 act[exn, io.err]
}

perm[-io.out] {
    // 所有在此块内定义的函数，即使自身声明了 io.out 也会被移除
    act[io.out, fs.write] restricted() { ... }  // 等效于 act[fs.write]
}
```

`perm` 块可以嵌套，内层权限在外层基础上继续合并：

```lumos
perm[exn] {
    act parse(string s) -> i32 { ... }      // act[exn]

    perm[+io.err] {
        act log_error(string s) { ... }     // act[exn, io.err]
    }
}
```

也可用于命名空间或类体内，以及 `impl` 块内：

```lumos
namespace io_ops {
    perm[exn] {
        act open(string path) -> File { ... }
        act read(File f) -> string { ... }
    }
    act sync(File f) { ... }  // 不带 exn
}

impl MyClass {
    perm[exn] {
        act open() -> File { ... }   // 等效于 act[exn]
        act close(File f) { ... }    // 等效于 act[exn]
    }
}
```

!!! note "与可执行块 `act[+xxx] { ... }` 的区别"
    `perm[...]` 是**声明块**，只允许出现在声明上下文中，块内只能包含函数/类型/变量声明，不能包含可执行语句。  
    `act[+xxx] { ... }` 是**执行块**，只出现在函数体等执行上下文中，块内是可以运行的语句序列。  
    两者用途完全不同，编译器根据上下文区分，不会产生歧义。

### 权限传播 (Yielding) {#yielding}

函数成功返回后，可以将权限自动应用到调用方后续的代码块中：

```lumos
act[fs.open] open_secure_file(string path) -> File yields [fs.read, fs.write] { ... }

act[fs.open] main() {
    val f = open_secure_file("/etc/config");
    // 此时 yields 的 [fs.read, fs.write] 自动生效
    f.write("data"); // OK
}
```

### 闭包与权限继承 {#closure-inheritance}

闭包可以使用 `def` / `fun` / `obs` / `act` 声明，`obs` Lambda 可在任意作用域定义，其权限由模块配额满足而非继承自调用方。`def` 作用域只能定义 `def` 闭包，`fun` 作用域只能定义 `fun` 闭包，`act` 作用域可以定义 `act` 闭包。  
当闭包使用 `act` 且省略权限时，闭包**自动继承其定义所在作用域的所有权限**。这遵循作用域继承原理，大幅简化常见用法。

> **效应多态**：如需接受比自身纯度更低的闭包（例如 `def` 函数接受 `act` 闭包），应将函数显式声明为 `def[%]`、`fun[%]` 或 `act[%]`。详见[效应多态](#polymorphism)。

#### 默认权限继承 {#default-inheritance}

```lumos
act[io.out, fs.write] process() {
    // 闭包 handler：act 必须，权限省略则默认继承 [io.out, fs.write]
    val handler = act (x: i32) -> i32 {
        println(`Processing $x`);  // 可使用 io.out
        return x * 2;
    };
    handler(42); // OK
}
```

#### 显式权限控制 {#explicit-permission-control}

可以在闭包定义时显式指定权限，覆盖继承的权限：

```lumos
act[io.out, fs.read, fs.write] main() {
    // 闭包 1: act[io.out]，显式指定权限，移除 fs.read 和 fs.write
    val logger = act[io.out] (msg: string) -> unit {
        println(msg);
        // fs.read();  // 编译错误：权限不足
    };

    // 闭包 2: act[]，表示没有任何编译器可静态追踪的副作用
    val pure_fn = act[] (x: i32) -> i32 {
        return x * 2;
        // println(x);  // 编译错误：没有 io.out 权限
    };

    // 闭包 3: act，权限省略则继承所有父权限
    val writer = act (path: string) -> unit {
        // 自动拥有 io.out, fs.read, fs.write
    };
}
```

### 效应多态 (%) {#polymorphism}

使用 `%` 占位符处理高阶函数，实现效应透明转发。`def[%]`、`fun[%]`、`act[%]` 均支持效应多态：带 `[%]` 的函数其调用点效应随传入闭包的效应浮动，副作用完全在**闭包生成处**和**调用方**两处限制，高阶函数体内无需感知。

#### 闭包参数的类型标注 {#closure-param-annotation}

`[%]` 作为参数类型时，前缀关键字决定该参数可以接受哪些纯度的闭包：

| 参数类型写法 | 可接受的闭包                   | 说明                                       |
|--------------|--------------------------------|--------------------------------------------|
| `act[%]`     | `def`、`fun`、`act[P]`（任意） | 最宽松，推荐用于 `act` 函数                |
| `fun[%]`     | `def`、`fun`（不含 `act`）     | 仅允许不携带 `act` 权限的闭包              |
| `def[%]`     | ——                             | **禁止**，`def` 已是最纯，下方无可多态之物 |

> `def[%]` 作为参数类型写法无意义，应直接写 `def`。编译器会报错。

#### 核心机制 {#core-mechanism}

`%` 是效应占位符，其含义为"此闭包参数的任意效应"。调用点的可观测效应为函数自身基线效应与闭包效应的上确界：

| 函数声明                  | 自身基线效应    | 传入 `def` 闭包 | 传入 `fun` 闭包 | 传入 `act[P]` 闭包 |
|---------------------------|-----------------|-----------------|-----------------|--------------------|
| `def[%] foo(act[%] f)`    | `def`（零效应） | `def`           | `fun`           | `act[P]`           |
| `fun[%] foo(act[%] f)`    | `fun`           | `fun`           | `fun`           | `act[P]`           |
| `act[Q][%] foo(act[%] f)` | `act[Q]`        | `act[Q]`        | `act[Q]`        | `act[Q ∪ P]`       |

**两处验证，HOF 体内零感知**：

- **闭包生成处**：决定闭包能捕获/使用什么效应
- **调用方**：调用点效应必须不超过调用方所处上下文的效应

```lumos
def[%] map</T, U/>([]T list, act[%](T) -> U f) -> []U { ... }

def pure_ctx() {
    map(list, def (x) -> x * 2);               // OK：调用效应 = def
    map(list, act (x) -> { println(x); x });    // 编译错误：act 闭包在 def 调用方中非法
}

act[io.out] act_ctx() {
    map(list, def (x) -> x * 2);               // OK：调用效应 = def
    map(list, act (x) -> { println(x); x });    // OK：调用效应 = act[io.out]
}
```

编译器自动追踪调用点闭包的效应，验证调用方是否满足要求；高阶函数自身的权限列表中无需重复列举所有可能的闭包权限。

```lumos
// 日志包装器：无需显式列举权限
act[io.out] logger_wrap(act[%] f) -> unit {
    println("Log: calling closure");
    f(); // 闭包权限由调用方提供
    println("Log: closure completed");
}

// 调用示例：简化的权限语法
act[io.out, fs.write] main() {
    // 闭包：act 必须，权限省略则继承 main 的 [io.out, fs.write]
    val handler = act (path: string) -> unit {
        println("Writing to: " + path);
        // write_file(path);
    };
    logger_wrap(handler); // OK：handler 的权限被 logger_wrap 接受
}
```

#### `def` 与 `def[%]` 的区别 {#def-vs-def-polymorphic}

> **无论传入什么闭包，普通 `def` 函数不能调用 `fun[%]` 函数；`def[%]` 可以。**

`fun[%]` 的基线效应是 `fun`，即使传入纯 `def` 闭包，它仍属于 `fun` 级别的函数。由于 `def` 只能调用 `def` 和 `obs`，所以 `def` 无法调用 `fun[%]`。`def[%]` 本身是效应多态的，可随传入闭包提升效应，因此调用 `fun[%]` 合法。

```lumos
fun[%] transform</T/>(T val, act[%](T) -> T f) -> T {
    return f(val);
}

def bad_caller() {
    transform(42, def (x) -> x * 2);    // 编译错误：def 不能调用 fun[%]
}

def[%] good_caller(act[%](i32) -> i32 f) -> i32 {
    return transform(42, f);             // OK：def[%] 可以调用 fun[%]
}
```

同理，`def` 不能调用 `act[%]`；`def[%]` 和 `fun[%]` 可以（若传入的闭包效应足够高）。

#### 无意义写法（编译器报错） {#meaningless-patterns}

`[%]` 的作用是让函数的调用点效应随传入的闭包浮动。若参数类型或函数声明使得浮动不可能发生，则该写法无意义，编译器应直接报错。

**规则：`X[%]` 函数声明有意义，当且仅当至少存在一个 `[%]` 闭包参数，其可接受的最高效应严格高于 `X` 的基线效应。**

| 无意义写法                            | 原因                                                         | 应改为                      |
|---------------------------------------|--------------------------------------------------------------|-----------------------------|
| `def[%]` 作为参数类型                 | `def` 已是最纯，无下方效应可多态                             | 改为 `def`                  |
| `fun[%] foo(fun[%] f)`                | `fun[%]` 参数最高带 `fun`，等于 `fun` 基线，`[%]` 永远不浮动 | 改为 `fun foo(fun[%] f)`    |
| `act[Q][%] foo(fun[%] f)`             | `fun[%]` 参数不携带 `act` 权限，无法在 `act[Q]` 基础上追加   | 改为 `act[Q] foo(fun[%] f)` |
| `X[%] foo(...)` 无任何 `[%]` 闭包参数 | `%` 无从追踪，声明上的 `[%]` 悬空                            | 去掉函数声明上的 `[%]`      |

```lumos
// ❌ 编译错误：def[%] 作为参数类型
fun[%] bad1(def[%](i32) -> i32 f) -> i32 { ... }

// ❌ 编译错误：fun[%] 函数的所有 [%] 参数最高仅为 fun，永不超出 fun 基线
fun[%] bad2(fun[%](i32) -> i32 f) -> i32 { ... }

// ❌ 编译错误：act[Q][%] 函数的 [%] 参数仅为 fun[%]，无法追加 act 权限
act[io.out][%] bad3(fun[%](i32) -> i32 f) -> i32 { ... }

// ❌ 编译错误：函数声明带 [%] 但没有任何 [%] 闭包参数
fun[%] bad4(i32 x) -> i32 { return x; }

// ✅ 正确：fun[%] 函数带 act[%] 参数，可浮动至 act
fun[%] ok1(act[%](i32) -> i32 f) -> i32 { ... }

// ✅ 正确：def[%] 函数带 fun[%] 参数，可浮动至 fun
def[%] ok2(fun[%](i32) -> i32 f) -> i32 { ... }
```

#### 通用高阶函数模式 {#generic-hof-patterns}

```lumos
// 通用 map：闭包自动继承调用方权限
act array_map</typename T, typename U/>(
    []T arr,
    act[%] (T) -> U transformer
) -> [100]U {
    var [100]U result = $[];
    for (i in arr) {
        result[i] = transformer(arr[i]); // 闭包权限由调用方提供
    }
    return result;
}

// 使用示例：无需重复声明权限
act[io.out] main() {
    val nums = $[1, 2, 3, 4, 5];

    // 闭包：act 必须，权限省略则自动继承 main 的 io.out 权限
    val doubled = array_map(nums, act (x: i32) -> i32 {
        println(`Processing $x`);
        return x * 2;
    });
}
```

#### 权限限制的高阶函数 {#restricted-hof}

即使调用方拥有更多权限，闭包仍可显式限制自身权限：

```lumos
// 高阶函数要求闭包是纯函数（无权限）
act safe_map</typename T, typename U/>(
    []T arr,
    act[] (T) -> U pure_fn  // 仅接受纯闭包
) -> [100]U {
    // 函数体...
}

act[io.out, fs.write] main() {
    val nums = $[1, 2, 3, 4, 5];

    // 闭包显式为 act[]，符合 safe_map 的要求
    val result = safe_map(nums, act[] (x: i32) -> i32 {
        return x * 2;  // 纯计算
    });

    // 以下会编译错误：闭包权限省略则继承 [io.out, fs.write]，不符合 act[] 要求
    // val bad = safe_map(nums, act (x: i32) -> i32 {
    //     println(x);  // 尝试使用继承的 io.out
    //     return x * 2;
    // });
}
```

#### 多闭包的权限追踪 {#multi-closure-tracking}

多个闭包各自追踪各自的权限。编译器会自动推断函数中有多少个 `act[%]` 参数，无需显式声明多个占位符：

```lumos
// 并行执行：只需声明一个 %，编译器自动推断有两个闭包
act parallel_execute(
    act[%] task_a,
    act[%] task_b
) -> unit {
    task_a();
    task_b();
}

act[io.out, fs.read, fs.write] main() {
    // task_a：act 权限省略，继承所有权限 [io.out, fs.read, fs.write]
    val reader = act (path: string) -> unit {
        // println(...); // 可用 io.out
        // read_file(path); // 可用 fs.read
    };

    // task_b：act[fs.write] 显式限制权限为仅 fs.write
    val writer = act[fs.write] (path: string) -> unit {
        // 只能使用 fs.write
    };

    parallel_execute(reader, writer); // 权限自动追踪
}
```

**自动推断规则**：

- 只需在闭包参数中使用 `act[%]`
- 编译器自动统计函数中有多少个 `act[%]` 参数
- 每个闭包的权限独立追踪，编译器在调用点验证每个闭包的权限充足性

#### 零成本抽象 {#zero-cost-abstraction}

- **编译期追踪**：闭包权限继承和 `%` 占位符完全在编译期处理
- **无运行时开销**：所有权限验证都在编译期完成
- **内联优化**：高阶函数可被优化器内联，闭包调用零开销

## 模块配额 {#module-quota}

在导入依赖库时分配权限上限：

```lumos
use http with [net]; 
```

任何违反配额的 `act[+xxx]` 都会导致编译错误。

---

相关内容：函数纯度见 [函数与纯度](function.md#title)。
