# 内存 `std.memory`

## 内存操作

### 内存填充 memset

将 n 个字节设置为指定值。

```lumos
// 字节级填充
@returns(dst)
@write(dst, n)
act[sys.mem] memset([unit] dst, u8 val, usize n) -> [unit];

// 8 位填充
act[sys.mem] memset8([u8] dst, u8 val, usize n) -> [unit];

// 16 位填充
act[sys.mem] memset16([u16] dst, u16 val, usize n) -> [unit];

// 32 位填充
act[sys.mem] memset32([u32] dst, u32 val, usize n) -> [unit];

// 64 位填充
act[sys.mem] memset64([u64] dst, u64 val, usize n) -> [unit];
```

### 内存复制 memcpy

将 n 个字节从源复制到目标。

```lumos
@returns(dst)
@read(src, n)
@write(dst, n)
act[sys.mem] memcpy([unit] dst, [unit] src, usize n) -> [unit];

// 无重叠安全复制
@returns(dst)
@read(src, n)
@write(dst, n)
act[sys.mem] memcpy_safe([unit] dst, [unit] src, usize n) -> [unit];
```

### 内存移动 memmove

支持源目标区域重叠的内存复制。

```lumos
@returns(dst)
@read(src, n)
@write(dst, n)
act[sys.mem] memmove([unit] dst, [unit] src, usize n) -> [unit];
```

### 内存比较 memcmp

比较两个内存区域。

```lumos
@read(a, n)
@read(b, n)
fun memcmp([unit] a, [unit] b, usize n) -> i32;  // 返回 -1, 0, 1

// 带长度限制的比较
fun memcmp_n([unit] a, [unit] b, usize n) -> i32;
```

## 指针操作

### 指针基础

```lumos
// 获取地址
fun address_of<typename T>(T value) -> [T];

// 解引用指针
fun deref<typename T>([T] ptr) -> T;

// 指针算术
fun \type> ptr_add<typename T>([T] ptr, usize offset) -> [T];
fun \type> ptr_sub<typename T>([T] ptr, usize offset) -> [T];

// 指针偏移量
fun offset_of<typename T>(usize field_offset) -> usize;

// 对齐信息
fun align_of<typename T>() -> usize;
```

### 空指针检查

```lumos
// 检查指针是否为空
fun is_null<typename T>([T] ptr) -> bool;

// 获取空指针
fun null<typename T>() -> [T];

// 有效指针检查
fun is_valid<typename T>([T] ptr) -> bool;
```

## 内存分配

### 堆分配

```lumos
// 分配未初始化内存
act[sys.mem] alloc(usize size) -> [unit];

// 分配并初始化为零
act[sys.mem] calloc(usize count, usize elem_size) -> [unit];

// 重新分配内存
act[sys.mem] realloc([unit] ptr, usize new_size) -> [unit];

// 释放内存
act[sys.mem] free([unit] ptr) -> unit;

// 分配对齐内存
act[sys.mem] aligned_alloc(usize alignment, usize size) -> [unit];
```

### 栈分配

```lumos
// 栈上分配（仅编译期已知大小）
fun alloca(usize size) -> [unit];

// 可变长栈数组（VLA）
fun vla<typename T>(usize count) -> [T];
```

## 内存布局和对齐

### 大小和对齐查询

```lumos
// 获取类型大小（字节）
fun sizeof<typename T>() -> usize;

// 获取类型对齐要求
fun alignof<typename T>() -> usize;

// 获取布局信息
\type> Layout {
    fun size() -> usize;
    fun align() -> usize;
    fun padding() -> usize;
}

fun layout_of<typename T>() -> \type> Layout;
```

### 字段偏移

```lumos
// 获取结构体字段的字节偏移量（编译期）
lit usize FIELD_OFFSET = offset_of!(struct, field);

// 运行时计算
fun offset_of_field(str struct_name, str field_name) -> usize;
```

## 智能指针

### 引用计数指针 Rc

```lumos
\type> Rc<typename T> {
    // 创建
    fun new(T value) -> \type> Rc<T>;

    // 解引用
    fun deref() -> T;

    // 克隆（增加引用计数）
    fun clone() -> \type> Rc<T>;

    // 获取引用计数
    fun ref_count() -> usize;

    // 尝试获取可变引用（仅当引用计数为1时）
    fun try_mut() -> Option<[T]>;
}
```

### 可变引用计数指针 Arc（原子）

```lumos
\type> Arc<typename T> {
    // 创建
    fun new(T value) -> \type> Arc<T>;

    // 解引用
    fun deref() -> T;

    // 克隆（线程安全）
    fun clone() -> \type> Arc<T>;

    // 获取引用计数
    fun ref_count() -> usize;

    // 尝试获取可变引用
    fun try_mut() -> Option<[T]>;
}
```

### 唯一指针 Box

```lumos
\type> Box<typename T> {
    // 创建
    fun new(T value) -> \type> Box<T>;

    // 解引用
    fun deref() -> T;

    // 转移所有权
    fun into_inner() -> T;

    // 泄漏内存（获取原始指针）
    fun leak() -> [T];
}
```

## 内存安全工具

### 生命周期管理

```lumos
// 检查悬垂指针
fun is_dangling<typename T>([T] ptr) -> bool;

// 获取指针指向的对象生命周期状态
\type> LifetimeInfo {
    fun is_valid() -> bool;
    fun allocation_size() -> usize;
    fun allocation_time() -> i64;
}

fun lifetime_info<typename T>([T] ptr) -> \type> LifetimeInfo;
```

### 内存泄漏检测

```lumos
// 启用内存泄漏检测
act[sys.mem] enable_leak_detection() -> unit;

// 禁用内存泄漏检测
act[sys.mem] disable_leak_detection() -> unit;

// 获取内存统计
\type> MemStats {
    fun allocated_bytes() -> u64;
    fun freed_bytes() -> u64;
    fun peak_bytes() -> u64;
    fun allocation_count() -> u64;
}

fun mem_stats() -> \type> MemStats;
```

## 使用示例

### 内存操作

```lumos
use std.memory.*;

act main() {
    // 填充内存
    var buffer = alloc(100);
    memset(buffer, 0, 100);

    // 复制数据
    var src = [1u8, 2u8, 3u8];
    var dst = alloc(3);
    memcpy(dst, address_of(src), 3);
}
```

### 指针操作

```lumos
use std.memory.*;

act main() {
    i32 x = 42;
    [i32] ptr = address_of(x);

    // 检查指针有效性
    if !is_null(ptr) {
        i32 value = deref(ptr);
        println("Value: {}", value);
    }
}
```

### 引用计数

```lumos
use std.memory.*;

act main() {
    var shared = Rc::new([1, 2, 3]);
    var copy1 = shared.clone();
    var copy2 = shared.clone();

    println("Ref count: {}", shared.ref_count());  // 3
}
```

### 内存统计

```lumos
use std.memory.*;

act main() {
    enable_leak_detection();

    // ... 你的代码 ...

    var stats = mem_stats();
    println("Allocated: {} bytes", stats.allocated_bytes());
    println("Peak: {} bytes", stats.peak_bytes());
}
```

## 指针标注

权限系统中的内存标注：

```lumos
// 标注可访问的字节数
@read(ptr, n)   // 标注读权限范围
@write(ptr, n)  // 标注写权限范围

// 标注不为空
@nonnull(ptr)   // 指针不为空
@nonnull(a, b)  // 多个指针都不为空

// 标注返回值
@returns(ptr)   // 返回指针本身

// 标注别名关系
@noalias(a, b)  // a 和 b 不相交
```

## 最佳实践

1. **使用智能指针**：优先 Box > Rc > Arc，避免手动管理
2. **避免指针算术**：除非必要，使用切片或容器
3. **启用检测**：开发阶段启用泄漏检测
4. **内存对齐**：大型数据结构注意对齐优化
5. **RAII 模式**：依靠析构函数自动释放资源

---

相关内容：变量修饰见 [变量与初始化](../grammar/variable.md#variables)。
