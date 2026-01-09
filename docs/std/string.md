# 字符串 `std.string`

## 字符串类型

```lumos
// 不可变字符串（栈分配）
str s = "hello";

// 可变字符串（堆分配）
var str s = "hello";

// 字符类型
u8 c = 'a';
```

## 字符串操作

### 基本操作

```lumos
// 字符串长度
usize len = s.length();

// 字符串容量
usize cap = s.capacity();

// 获取字符
u8 ch = s.at(0);

// 子字符串
str sub = s.substring(0, 5);
```

### 搜索和匹配

```lumos
// 查找子字符串
[usize] indices = s.find("lo");

// 判断包含
fun contains(str substr) -> bool;

// 判断开头
fun starts_with(str prefix) -> bool;

// 判断结尾
fun ends_with(str suffix) -> bool;

// 首次出现位置
fun index_of(str substr) -> usize;

// 最后一次出现位置
fun last_index_of(str substr) -> usize;
```

### 转换操作

```lumos
// 转换为大写
fun to_upper() -> str;

// 转换为小写
fun to_lower() -> str;

// 去除空格
fun trim() -> str;

// 去除左空格
fun trim_start() -> str;

// 去除右空格
fun trim_end() -> str;

// 替换
fun replace(str old, str new) -> str;

// 分割
fun split(str delimiter) -> [str];

// 连接
fun join([str] parts, str delimiter) -> str;
```

### 类型转换

```lumos
// 字符串转整数
fun \type> to_i32(str s) -> i32;
fun \type> to_i64(str s) -> i64;
fun \type> to_u32(str s) -> u32;
fun \type> to_u64(str s) -> u64;

// 字符串转浮点
fun \type> to_f32(str s) -> f32;
fun \type> to_f64(str s) -> f64;

// 字符串转布尔
fun \type> to_bool(str s) -> bool;

// 整数转字符串
fun \type> from_i32(i32 value) -> str;
fun \type> from_i64(i64 value) -> str;
fun \type> from_u32(u32 value) -> str;
fun \type> from_u64(u64 value) -> str;

// 浮点转字符串
fun \type> from_f32(f32 value) -> str;
fun \type> from_f64(f64 value) -> str;

// 格式化字符串
fun format(str fmt, ...) -> str;
```

### 可变字符串操作

```lumos
// 追加字符
fun push(u8 ch) -> unit;

// 追加字符串
fun append(str s) -> unit;

// 前置插入
fun prepend(str s) -> unit;

// 插入字符
fun insert(usize pos, u8 ch) -> unit;

// 删除字符
fun remove(usize pos) -> u8;

// 清空
fun clear() -> unit;

// 扩展容量
fun reserve(usize capacity) -> unit;

// 缩小容量
fun shrink() -> unit;
```

## 使用示例

```lumos
use std.string.*;

act main() {
    // 创建不可变字符串
    str greeting = "Hello, World!";
    println(greeting);

    // 创建可变字符串
    var str name = "Alice";
    name.append(" Smith");
    println(name);

    // 搜索操作
    if greeting.contains("World") {
        println("Found!");
    }

    // 转换
    i32 num = greeting.find("o")[0];
    println(num);

    // 分割
    [str] parts = "a,b,c".split(",");
    for part in parts {
        println(part);
    }
}
```
