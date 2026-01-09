# 副作用权限系统 (Side-Effect Permission System) {#permission-system-side-effect-permission-system}

Lumos 采用静态效应系统，将副作用视为一种受控的“能力 (Capability)”。

## 核心原则 {#core-principles}

- **零信任**：`act main` 默认没有任何权限。
- **静态检查**：所有权限在编译期校验，无运行时开销。
- **显式授权**：副作用必须在 `act` 函数签名或代码块上显式标注。
- **纯度隔离**：`def` 和 `fun` 权限集恒为空（仅内置 `dbg`），只有 `act` 可以声明权限。

## 权限层级 {#permission-hierarchy}

权限采用点号分隔的树状结构。完整的权限树定义请参考 [permissions.yaml](permissions.yaml)。

### 常用内置权限 {#built-in-permissions}

| 权限路径          | 别名 (aka) | 描述            |
|-------------------|------------|-----------------|
| `io.out`          | `stdout`   | 标准输出流      |
| `io.err`          | `stderr`   | 标准错误流      |
| `io.in`           | `stdin`    | 标准输入流      |
| `fs.read`         | -          | 文件读取        |
| `fs.write`        | -          | 文件写入        |
| `net.http.client` | -          | HTTP 客户端请求 |
| `sys.env`         | -          | 访问环境变量    |

> **提示**：别名（如 `stdout`）在代码中与完整路径（如 `io.out`）完全等价。

### 权限定义与别名 {#permission-aliases}

库或模块可以定义自己的权限，并使用 `aka` 关键字提供简写或兼容性名称：

```lumos
permission my_lib.network.socket aka socket;
```

- 拥有父权限等价于拥有其下所有子权限。

支持组语法：
`act[net.http{client, server}]` 等价于 `act[net.http.client, net.http.server]`。

## 权限操作 {#permission-operations}

权限控制是代码块级别的，子块默认继承父块权限。

- **`act[+xxx];` (提升当前块)**：在当前代码块后续部分增加权限。
- **`act[+xxx] { ... }` (提升子块)**：仅在指定的子块中增加权限。
- **`act[-xxx]` (屏蔽当前块)**：在当前块及其子块中屏蔽特定权限。
- **`act[-xxx] { ... }` (屏蔽子块)**：仅在指定的子块中屏蔽特定权限。
- **`act[xxx]` (重置当前块)**：当前块仅保留指定的权限，其余继承来的权限全部丢弃。
- **`act[xxx] { ... }` (重置子块)**：仅在指定的子块中重置权限。
- **`act[*, "reason"]` (逃生舱)**：强制获得所有权限。要求提供字符串理由，便于审计。
- **`act[*, "reason"] { ... }` (子块逃生舱)**：仅在指定子块中强制获得所有权限。要求提供字符串理由，便于审计。
- **`act[?]`**：编译器指令。在编译时打印当前代码块所拥有的完整权限树。

## 函数权限 {#function-permissions}

### 权限声明 {#permission-declaration}

```lumos
act[net.http.client] fetch_data(string url) -> string { ... }
```

如果函数无需任何权限，直接使用 `act` 关键字即可：

```lumos
act main() { ... }
```

### 权限传播 (Yielding) {#permission-yielding-yielding}

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

闭包必须使用 `act` 关键字声明，但可以省略权限声明。当权限省略时，闭包**自动继承其定义所在作用域的所有权限**。这遵循作用域继承原理，大幅简化常见用法。

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

    // 闭包 2: act[]，表示没有任何权限（纯函数）
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

使用 `%` 占位符处理高阶函数，实现权限透明转发。编译器会自动追踪通过闭包参数传入的权限需求，无需在高阶函数签名中显式列举所有可能的权限。

#### 核心机制 {#core-mechanism}

`%` 是一个权限占位符，其含义为："此闭包所需的任意权限"。编译器会自动追踪调用点闭包的权限，验证调用方是否拥有。

**关键点**：

- 闭包参数 `act[%]` 表示接收任意权限的闭包
- 高阶函数可不显式声明权限，直接接收闭包权限
- 编译器在调用点自动验证权限充足性

```lumos
// 日志包装器：无需显式列举权限
act[io.out, %] logger_wrap(act[%] f) -> unit {
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

#### 通用高阶函数模式 {#generic-hof-patterns}

```lumos
// 通用 map：闭包自动继承调用方权限
act[io.out, %] array_map</typename T, typename U/>(
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
act[io.out, fs.write, %] safe_map</typename T, typename U/>(
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
act[%] parallel_execute(
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

- 只需在高阶函数签名中声明一个 `%`
- 编译器自动统计函数中有多少个 `act[%]` 参数
- 每个闭包的权限独立追踪，编译器在调用点验证每个闭包的权限充足性

#### 零成本抽象 {#zero-cost-abstraction}

- **编译期追踪**：闭包权限继承和 `%` 占位符完全在编译期处理
- **无运行时开销**：所有权限验证都在编译时完成
- **内联优化**：高阶函数可被优化器内联，闭包调用零开销

## 模块配额 {#module-quota}

在导入依赖库时分配权限上限：

```lumos
use http with [net]; 
```

任何违反配额的 `act[+xxx]` 都会导致编译错误。

---

相关内容：函数纯度见 [函数与纯度](function.md#functions)。
