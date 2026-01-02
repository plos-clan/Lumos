# 输入输出 `std.io`

## 默认导入

标准库提供以下默认导入：

```lumos
using \type> io.\act> print;
using \type> io.\act> println;
using \type> io.\act> scan;
using \type> io.\act> scanln;
```

## 基础输出

### 简单输出

```lumos
// 输出不换行
act[io.out] print(...) -> unit;

// 输出并换行
act[io.out] println(...) -> unit;

// 输出单个字符
act[io.out] putchar(u8 ch) -> unit;

// 输出字符串
act[io.out] puts(str s) -> unit;
```

### 格式化输出

```lumos
// 格式化打印（不换行）
act[io.out] printf(str fmt, ...) -> unit;

// 格式化打印（换行）
act[io.out] printfln(str fmt, ...) -> unit;

// 转换为格式化字符串
fun format(str fmt, ...) -> str;

// 带缓冲的格式化输出
act[io.out] vprintf(str fmt, [unit] args) -> unit;
```

**格式化符号**：

| 符号 | 类型 | 示例 |
|------|------|------|
| `{}` | 自动推导 | `print("Value: {}", 42)` |
| `{d}` | 十进制整数 | `print("{d}", 42)` |
| `{x}` | 十六进制 | `print("{x}", 255)` |
| `{o}` | 八进制 | `print("{o}", 8)` |
| `{b}` | 二进制 | `print("{b}", 5)` |
| `{f}` | 浮点数 | `print("{f}", 3.14)` |
| `{e}` | 科学计数法 | `print("{e}", 1000.0)` |
| `{s}` | 字符串 | `print("{s}", "hello")` |
| `{p}` | 指针 | `print("{p}", ptr)` |

**宽度与精度**：

```lumos
print("{:10d}", 42);       // 右对齐宽度10
print("{:<10d}", 42);      // 左对齐宽度10
print("{:^10d}", 42);      // 居中对齐宽度10
print("{:0>10d}", 42);     // 零填充
print("{:.2f}", 3.14159);  // 浮点精度2
```

## 基础输入

### 简单输入

```lumos
// 输入一个字符
fun scan_char() -> u8;

// 输入一行（去除末尾换行）
fun scan_line() -> str;

// 输入整数
fun scan_i32() -> i32;
fun scan_i64() -> i64;
fun scan_u32() -> u32;
fun scan_u64() -> u64;

// 输入浮点数
fun scan_f32() -> f32;
fun scan_f64() -> f64;

// 输入字符串
fun scan_str() -> str;
```

### 格式化输入

```lumos
// 格式化扫描
fun scanf(str fmt, ...) -> usize;  // 返回成功读入的项数

// 带缓冲的格式化输入
fun vscanf(str fmt, [unit] args) -> usize;
```

**格式化符号**：

```lumos
scanf("{d}", a);         // 读整数
scanf("{f}", b);         // 读浮点
scanf("{s}", str);       // 读字符串（跳过空格）
scanf("%[^\n]", line);   // 读整行（包括空格）
scanf("%c", ch);         // 读字符
```

### 通用扫描接口

```lumos
// 通用扫描（类型推导）
fun scan<typename T>() -> T;

// 扫描带默认值
fun scan_or<typename T>(T default) -> T;

// 扫描多个值
fun scan_tuple<typename ...T>() -> (...T);
```

使用示例：

```lumos
i32 x = scan<i32>();              // 读入整数
str name = scan_or<str>("Unknown"); // 读入字符串，默认"Unknown"
```

## 输出流

### 标准流操作

```lumos
\type> OutputStream {
    // 写字符
    act[io.out] write(u8 ch) -> unit;

    // 写字节数据
    act[io.out] write_bytes([u8] data) -> usize;

    // 写字符串
    act[io.out] write_str(str s) -> usize;

    // 刷新缓冲
    act[io.out] flush() -> unit;

    // 关闭流
    act[io.out] close() -> unit;

    // 流大小（字节数）
    fun size() -> usize;
}
```

### 标准流对象

```lumos
// 标准输出流
val \type> stdout: OutputStream;

// 标准错误流
val \type> stderr: OutputStream;

// 标准日志流（带时间戳）
val \type> stdlog: OutputStream;
```

### 流输出示例

```lumos
using std.io.*;

act main() {
    stdout.write_str("Hello, ");
    stdout.write_str("World!");
    stdout.write('\n' as u8);
    stdout.flush();
}
```

## 输入流

### 标准输入流操作

```lumos
\type> InputStream {
    // 读字符
    fun read_char() -> Option<u8>;

    // 读指定字节数
    fun read_bytes(usize n) -> [u8];

    // 读一行
    fun read_line() -> Option<str>;

    // 读全部内容
    fun read_all() -> str;

    // 关闭流
    act[io.in] close() -> unit;

    // 检查是否达到文件结尾
    fun is_eof() -> bool;
}
```

### 标准输入对象

```lumos
// 标准输入流
val \type> stdin: InputStream;
```

## 缓冲化控制

### 缓冲设置

```lumos
// 全缓冲（直到缓冲满或显式刷新）
fun set_full_buffer(usize size) -> unit;

// 行缓冲（每行刷新）
act[io.out] set_line_buffer() -> unit;

// 无缓冲（立即输出）
act[io.out] set_no_buffer() -> unit;
```

## 高级特性

### 颜色和样式输出

```lumos
// ANSI 颜色
fun color_red() -> str;
fun color_green() -> str;
fun color_blue() -> str;
fun color_yellow() -> str;
fun color_reset() -> str;

// 使用颜色
print("{}Error: Something failed{}", color_red(), color_reset());
```

### 进度显示

```lumos
// 进度条
act[io.out] progress_bar(usize current, usize total, usize width) -> unit;

// 旋转动画
act[io.out] spinner(usize step) -> unit;
```

### 表格输出

```lumos
\type> Table {
    fun new() -> \type> Table;
    fun add_row([str] row) -> unit;
    fun set_header([str] header) -> unit;
    act[io.out] print() -> unit;
}

// 使用示例
var table = Table::new();
table.set_header(["Name", "Age", "City"]);
table.add_row(["Alice", "25", "NYC"]);
table.add_row(["Bob", "30", "LA"]);
table.print();
```

## 使用示例

### 基础 I/O

```lumos
using std.io.*;

act main() {
    // 输出
    println("Hello, Lumos!");

    // 输入
    print("Enter your name: ");
    str name = scan_line();

    print("Enter your age: ");
    i32 age = scan<i32>();

    // 格式化输出
    println("Name: {}, Age: {}", name, age);
}
```

### 格式化输出

```lumos
using std.io.*;

act main() {
    i32 x = 42;
    f32 pi = 3.14159;

    println("Decimal: {d}", x);
    println("Hex: {x}", x);
    println("Binary: {b}", x);
    println("Float: {:.3f}", pi);
    println("Aligned: {:>10d}", x);
}
```

### 文件路径表示

```lumos
using std.io.*;
using std.string.*;

act main() {
    // 错误消息到 stderr
    stderr.write_str("Warning: File not found\n");

    // 日志输出
    stdlog.write_str("Application started\n");
    stdlog.flush();
}
```
