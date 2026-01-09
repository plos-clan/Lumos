# 标准库

!!! tip "关于标准库对名称的规定"
    标准库规定了许多变量及函数的用法，你不仅应该在标准库中这样使用这些函数，也应该在自己的代码中这样使用。  
    这样可以使你的代码更加规范，也可以使你的代码更加易读。

!!! note "这些命名标准的意义"
    众所周知当你开始编写一个项目时，最困难的一件事就是给变量和函数命名。  
    但是当你使用统一的命名标准时，你不需要再记住各个函数的作用，你只需要记住标准规定就可以命名和使用函数和变量。

!!! note "关于异常"
    标准库的每个函数都有开启异常和关闭异常的版本。因为 Lumos 默认是关闭异常的。  
    为了在全局开启异常，你可以编辑项目根目录的 `lumos.yaml`/`lumos.json`/`lumos.toml`（三选一）。

## 核心功能

### [内存 `std.memory`](memory.md)

低级内存操作、指针管理和智能指针。

- `memset`, `memcpy`, `memmove`, `memcmp`
- 指针操作和检查
- 内存分配和释放
- 引用计数和智能指针（Rc, Arc, Box）
- 内存统计和泄漏检测

### [字符串 `std.string`](string.md)

字符串操作和文本处理。

- 字符串创建和基本操作
- 搜索、匹配和替换
- 字符串转换和分割
- 类型转换（字符串↔数字）
- 可变字符串操作

### [容器 `std.container`](container.md)

数据结构和容器类型。

- **有序集**：Array, List, LinkedList, Queue, Stack
- **无序集**：HashSet
- **排序集**：Set, MinPQ, MaxPQ
- **映射**：HashMap, Map, ArrayMap, ListMap 等
- 迭代器接口
- 容器通用操作

## 系统交互

### [输入输出 `std.io`](io.md)

标准输入输出和流操作。

- 基础输出：`print`, `println`
- 格式化输出：`printf`, `format`
- 基础输入：`scan`, `scan_line`, `scan_str`
- 格式化输入：`scanf`
- 流对象：stdin, stdout, stderr
- 缓冲化控制
- 颜色和表格输出

### [文件操作 `std.file`](file.md)

文件和目录操作。

- 文件打开、读取、写入
- 文件指针操作（seek）
- 文件元数据查询
- 目录操作：创建、列表、遍历
- 文件系统操作：复制、移动、删除
- 路径处理和规范化
- 临时文件和目录

### [线程 `std.thread`](thread.md)

并发编程和线程安全。

- 线程创建和管理
- 互斥锁（Mutex）
- 读写锁（RWLock）
- 条件变量（Condvar）
- 屏障（Barrier）
- 自旋锁（Spin）
- 原子类型（Atomic）
- 通道（Channel）
- 线程池

### [随机数 `std.random`](random.md)

伪随机数生成。

- 全局随机函数
- 随机数生成器类（RNG）
- 各种分布：均匀、正态、指数、泊松、二项
- 性能优化建议

## 算法

### [算法 `std.algorithm`](algorithm.md)

常用算法库。

- **搜索**：线性搜索、二分搜索
- **排序**：快速排序、合并排序、堆排序
- **变换**：映射、过滤、反转、旋转
- **比较**：集合操作、合并
- **分区**：数据分区和排序
- **数值**：累积和、最大最小值、求和

## 模块导入

### 导入整个模块

```lumos
using std.container.*;
using std.algorithm.*;
using std.string.*;
```

### 导入特定函数

```lumos
using std.io.println;
using std.io.print;
using std.random.rand;
using std.file.open_read;
```

### 有限导入

```lumos
// 导入容器模块，但不导入内部的私有实现
using std.container.{List, HashMap, Set};
```

## 快速参考

### I/O 操作

```lumos
println("Hello, {}!", name);                  // 带格式化输出
i32 x = scan<i32>();                         // 读整数
str line = scan_line();                       // 读一行
```

### 容器操作

```lumos
var list = List::<i32>::new();
list.push(1);
list.push(2);

val map = HashMap::<str, i32>::new();
map.set("age", 25);
```

### 字符串操作

```lumos
str s = "hello world";
str upper = s.to_upper();
[str] parts = s.split(" ");
str formatted = format("Value: {}", 42);
```

### 文件操作

```lumos
match open_read("data.txt") {
    Ok(file) => {
        str content = file.read_all().unwrap();
        file.close();
    }
    Err(err) => println("Error: {}", err),
}
```

### 并发编程

```lumos
val counter = Mutex::new(0);
spawn {
    var guard = counter.lock();
    guard[] = guard[] + 1;
};
```

### 随机数

```lumos
srand_auto();
i32 num = rand_range(1, 100);
f32 prob = randf();
```

## 性能提示

1. **容器选择**
   - 频繁随机访问：List
   - 频繁两端操作：LinkedList 或 Deque
   - 快速搜索：HashMap, Set
   - 有序遍历：Map

2. **内存管理**
   - 避免频繁分配：预分配足够容量
   - 使用智能指针而非原始指针
   - 启用检测工具捕获泄漏

3. **I/O 性能**
   - 批量读写而非单个字符
   - 适当调整缓冲区大小
   - 大文件考虑流式处理

4. **算法选择**
   - 小数据集：简单排序（插入排序）
   - 一般情况：快速排序
   - 需要稳定性：合并排序
   - 无锁场景：原子操作

## 标准库的扩展

Lumos 社区提供了许多标准库扩展：

- 数学库：矩阵、线性代数
- 日期时间库：时间戳、时区
- 正则表达式库
- JSON/YAML 序列化库
- 网络库：TCP, UDP, HTTP

这些库可通过包管理系统安装。
