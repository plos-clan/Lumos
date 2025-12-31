# 副作用权限系统 (Side-Effect Permission System)

Lumos 采用静态效应系统，将副作用视为一种受控的“能力 (Capability)”。

## 1. 核心原则

- **零信任**：`act main` 默认没有任何权限。
- **静态检查**：所有权限在编译期校验，无运行时开销。
- **显式授权**：副作用必须在 `act` 函数签名或代码块上显式标注。
- **纯度隔离**：`def` 和 `fun` 权限集恒为空（仅内置 `dbg`），只有 `act` 可以声明权限。

## 2. 权限层级

权限采用点号分隔的树状结构。完整的权限树定义请参考 [permissions.yaml](permissions.yaml)。

### 2.1 常用内置权限

| 权限路径 | 别名 (aka) | 描述 |
| :--- | :--- | :--- |
| `io.out` | `stdout` | 标准输出流 |
| `io.err` | `stderr` | 标准错误流 |
| `io.in` | `stdin` | 标准输入流 |
| `fs.read` | - | 文件读取 |
| `fs.write` | - | 文件写入 |
| `net.http.client` | - | HTTP 客户端请求 |
| `sys.env` | - | 访问环境变量 |

> **提示**：别名（如 `stdout`）在代码中与完整路径（如 `io.out`）完全等价。

### 2.2 权限定义与别名

库或模块可以定义自己的权限，并使用 `aka` 关键字提供简写或兼容性名称：

```lumos
permission my_lib.network.socket aka socket;
```

- 拥有父权限等价于拥有其下所有子权限。

支持组语法：
`act[net.http{client, server}]` 等价于 `act[net.http.client, net.http.server]`。

## 3. 权限操作

权限控制是代码块级别的，子块默认继承父块权限。

- **`act[+xxx];` (提升当前块)**：在当前代码块后续部分增加权限。
- **`act[+xxx] { ... }` (提升子块)**：仅在指定的子块中增加权限。
- **`act[-xxx]` (屏蔽)**：在当前块及其子块中屏蔽特定权限。
- **`act[xxx]` (重置)**：当前块仅保留指定的权限，其余继承来的权限全部丢弃。
- **`act[*, "reason"]` (逃生舱)**：强制获得所有权限。要求提供字符串理由，便于审计。
- **`act[?]`**：编译器指令。在编译时打印当前代码块所拥有的完整权限树。

## 4. 函数权限

### 4.1 权限声明

```lumos
act[net.http.client] fetch_data(string url) -> string { ... }
```

如果函数无需任何权限，直接使用 `act` 关键字即可：

```lumos
act main() { ... }
```

### 4.2 权限传播 (Yielding)

函数成功返回后，可以将权限自动应用到调用方后续的代码块中：

```lumos
act[fs.open] open_secure_file(string path) -> File yields [file_io] { ... }

act[fs.open] main() {
    val f = open_secure_file("/etc/config");
    // 此时 yields 的 [file_io] 自动生效
    f.write("data"); // OK
}
```

### 4.3 效应多态 (%)

使用 `%` 占位符处理高阶函数，实现权限透明转发：

```lumos
act[io.out, %] logger_wrap(act[%] f) -> unit {
    println("Log: before call");
    f(); // 调用闭包，消耗 % 权限
}
```

## 5. 模块配额

在导入依赖库时分配权限上限：

```lumos
using http with [net]; 
```

任何违反配额的 `act[+xxx]` 都会导致编译错误。
