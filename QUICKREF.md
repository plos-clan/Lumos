# Lumos 语言速览 (Quick Reference)

> **项目状态**：纯文档设计阶段，编译器尚未实现。完整文档见 `docs/` 目录。

---

## 目录

- [项目结构](#项目结构)
- [最小可运行程序](#最小可运行程序)
- [函数纯度系统](#函数纯度系统)
- [变量声明](#变量声明)
- [类型系统](#类型系统)
- [权限系统](#权限系统)
- [控制流程](#控制流程)
- [运算符](#运算符)
- [类与结构体](#类与结构体)
- [枚举](#枚举)
- [模板与泛型](#模板与泛型)
- [异常处理](#异常处理)
- [模块系统](#模块系统)
- [计量单位](#计量单位)
- [词法规则](#词法规则)
- [标准库速查](#标准库速查)
- [文档导航](#文档导航)

---

## 项目结构

```
docs/
  home/       # 项目主页、状态、阅读路线、语法速览
  grammar/    # 语言核心语法（函数、变量、类型、控制流等）
  lexical/    # 词法（字符串、数字字面量、注释）
  std/        # 标准库设计
  ast/        # 抽象语法树定义（Rust 描述）
  ir/         # 中间表示 (LIR / SSA)
  macro/      # 宏系统
  examples/   # 示例代码（带注释）
example/      # .lm 示例源文件
```

---

## 最小可运行程序

```lumos
use "std" -> std;

act[io.out] main() -> i32 {
  std.println("Hello, Lumos!");
  return 0;
}
```

---

## 函数纯度系统

| 关键字 | 含义 | 可调用 | 备注 |
|--------|------|--------|------|
| `def`  | 纯函数，无任何副作用 | `def`, `obs` | 单语句可写 `= 表达式` |
| `fun`  | 逻辑纯函数，外部不可观测副作用 | `fun`, `def`, `obs` | 可调用 `unsafe fun[]` |
| `obs`  | 观测性函数，调用方无需持有其权限 | 任意 | 返回类型必须为 `unit` |
| `act`  | 副作用函数，必须声明权限 `[...]` | 任意 | I/O、全局状态等 |

```lumos
def add(i32 a, i32 b) -> i32 = a + b;   // 纯函数

obs[io.err] log(string msg) {           // 观测性：调用方不需要 io.err
  stderr.write(msg);
}

fun compute(i32 x) -> i32 {
  log(`input: $x`);                     // OK：fun 可调用 obs
  return add(x, 1);
}

act[io.out] main() -> i32 {
  std.println(compute(41));
  return 0;
}
```

### `unsafe fun` 与回滚块

```lumos
unsafe fun[] next_id() -> i32 {
  @static
  var i32 counter = 0;
  counter = counter + 1;
  return counter;
} rollback {
  counter = counter - 1;     // 调用链后续抛异常时逆序触发
}
```

### 修饰符

- `ovl`：允许重载（所有同名定义都必须标记）
- `once? act` / `once! act`：仅执行一次（再调用时忽略/报错）
- `async fun` / `async act`：异步函数
- `dbg(...)`：调试输出，发布模式移除，不破坏纯度

---

## 变量声明

| 关键字 | 语义 | 内存 |
|--------|------|------|
| `var`  | 完全可变（隐含 `mut`） | 读写内存 |
| `val` / 裸类型 | 逻辑不可变 | 内部可变性允许（如懒加载） |
| `imv`  | 物理不可变（隐含 `imm`） | 可放 `.rodata` |
| `lit`  | 编译期常量，无运行时地址 | 常量折叠内联 |

```lumos
var i32 a = 1;        // 完全可变
val i32 b = 2;        // 逻辑不可变（val 可省略，写 i32 b = 2;）
imv i32 c = 3;        // 物理不可变
lit i32 MAX = 100;    // 编译期常量
```

### 延迟初始化

```lumos
val CONF = late i32;     // late：手动初始化（访问前必须赋值）
CONF = 42;

val CACHE = lazy {       // lazy：首次访问时自动初始化（线程安全）
  compute_value()
};
```

### 其他修饰符

- `fin`：禁止在同一作用域内重新绑定（shadowing）
- `volatile`：禁止编译器缓存优化
- `restrict`：指针独占限定（类似 C99 restrict）
- `with var x = val; { ... }`：临时作用域变量

### 访问控制

`ro`（只读）/ `wo`（只写）/ `rw`（读写，默认）/ `rx`（读执行）

---

## 类型系统

### 基础类型

| 类型 | 说明 |
|------|------|
| `unit` | 空类型，同时也是其唯一值字面量 |
| `bool` | 布尔（`true`/`false`） |
| `flag` | 标志位（`on`/`off`，不能用于条件判断） |
| `char` | Unicode 字符（UTF-32 存储） |
| `i8..i128`, `u8..u128` | 有/无符号整数 |
| `f16..f128` | 浮点数 |
| `isize`, `usize` | 平台字长整数 |
| `bN` | 位操作专用类型（仅允许位运算） |

### 字符串类型

| 类型 | 说明 |
|------|------|
| `str` / `str8/16/32` | 默认/UTF 编码字符串（逻辑不可变） |
| `cstr` / `cstr8/16/32` | C 风格以 `\0` 结尾字符串 |
| `bytes` | 字节数组，无编码约束 |

### 指针与引用

```lumos
[i32] a    // 可空指针（nullable pointer）
&i32 b     // 非空引用（non-null）
!i32 c     // 唯一引用（unique/owned）
[N]i32 d   // 固定长度数组
[]i32 e    // 切片（slice）
```

### 类型操作

```lumos
a as f64           // 类型转换
typeof(a)          // 获取类型（可直接用作类型名）
typenameof(a)      // 获取类型名字符串
sizeof(a)          // 获取大小
a is i32           // 类型判断
```

---

## 权限系统

```lumos
act[io.out, fs.read] my_func() { ... }   // 声明权限
act[net.http{client, server}] fetch() {}  // 组语法
```

### 常用内置权限

| 权限 | 说明 |
|------|------|
| `io.out` / `stdout` | 标准输出 |
| `io.err` / `stderr` | 标准错误 |
| `io.in` / `stdin` | 标准输入 |
| `fs.read` / `fs.write` | 文件读写 |
| `net.http.client` | HTTP 客户端 |
| `sys.env` | 环境变量 |
| `exn` | 允许异常传播（栈展开） |

### 代码块级权限操作

```lumos
act[+io.out];           // 提升当前块权限
act[-fs.write] { ... } // 子块中屏蔽权限
act[io.out] { ... }    // 子块中重置权限（仅保留指定）
act[*, "reason"] { ... } // 逃生舱：获得所有权限（需说明理由）
act[?]                  // 打印当前权限树（编译期调试）
```

### 声明块批量权限

```lumos
perm[exn] {
  act open(string path) -> File { ... }  // 等效 act[exn]
  act read(File f) -> string { ... }     // 等效 act[exn]
}
```

### 效应多态

```lumos
def[%] map</T, U/>([]T list, act[%](T) -> U f) -> []U { ... }
// act[%] 参数可接受任意纯度的闭包；调用点效应随闭包浮动
```

---

## 控制流程

### 分支

```lumos
if (x > 0) { ... } elif (x == 0) { ... } else { ... }

// if 表达式
i32 abs = if (x >= 0) x else -x;

// 后置 if
return -1 if error;
x > 0 then println("positive");

// switch（无需 break）
switch (code) {
  200: { ... }
  404: fallthrough;   // 显式贯穿
  500: { ... }
  else: leave;        // 跳出 switch
}

// match（条件匹配）
match (x) {
  _ > 0: { ... }
  else:  { ... }
}
```

### 循环

```lumos
for (var i32 i = 0; i < 10; i += 1) { ... }
for (val item in collection) { ... }
for (5) { ... }           // 执行 5 次
while (cond) { ... }
do { ... } while (cond);

// 循环标签
outer:
for (...) {
  for (...) { break outer; }
}

// 循环后处理
for (...) { ... }
} broken { ... }   // break 时执行
} then   { ... }   // 正常结束时执行
```

### 跳转

```lumos
break [label]   // 跳出循环（支持标签）
continue        // 跳过本次
leave           // 跳出当前 {} 块（非函数体）
return 值       // 函数返回
goto label      // 标签跳转
goto 0x1234     // 地址跳转
assume cond;    // 向优化器声明条件成立（违反→UB）
```

---

## 运算符

### 算术

```lumos
a + b  a - b  a * b  a / b  a % b  // 标准（% = rem，符号同被除数）
a.mod(b)                             // 欧式取模（符号同除数）
a /% b  // divrem，返回 (商, 余数) 元组
a /+ b  // divceil（向上取整除法）
a /- b  // divfloor（向下取整除法）
```

### 溢出控制（整数）

```lumos
a +^ b  // 饱和加法（saturate）
a +% b  // 环绕加法（wrapping）
a +! b  // 进位加法（返回 (result, carry) 元组）
// 同理：-^/-% /-!、*^/*%/*!
```

### 位运算

```lumos
a & b  a | b  a ^ b        // 按位与、或、异或
a << b  a >> b              // 左移、右移
a <<< b  a >>> b            // 无符号左移、右移
a <<> b  a >>< b            // 循环左移、右移
```

### 逻辑与比较

```lumos
a and b  a or b  a xor b   // 逻辑运算（有短路）
!a  not a                   // 逻辑取反
a == b  a != b  a < b  a <= b  a > b  a >= b
a <=> b                     // 三路比较（-1/0/1）
a === b  a !== b             // 严格相等（可重载）
a $== b                     // 二进制相等（不可重载）
a &== b                     // 同一对象（引用相等）
```

### 其他

```lumos
a as T                   // 类型转换
my_func(a?) : fallback   // 可空传播（a 为 null 则返回 fallback）
sizeof(a)  typeof(a)  typenameof(a)
```

### 运算符重载

```lumos
def \add(T rhs) -> T { ... }   // 重载 +
def \cmp(T rhs) -> i32 { ... } // 重载 <=>（自动生成 ==/</>/<= 等）
def \hash() -> usize { ... }   // 重载哈希
```

---

## 类与结构体

```lumos
class MyClass {               // class：完整 OOP（支持虚函数、多态）
  pub i32 x;                  // pub：公共；prv：私有；默认：受保护
  i32 y;                      // 默认受保护
}

struct Point { i32 x, y; }    // struct：无虚函数/多态，轻量数据

impl MyClass {                 // 外部实现方法
  pub def get_x() -> i32 { return x; }
  pub act@wo set_x(i32 v) { x = v; }  // @wo：只写成员
}
```

### 成员函数访问修饰

`@ro`（只读成员）/ `@wo`（只写成员）/ `@rw`（读写，默认）/ `@rx`（读执行）

### 类型转换重载

```lumos
def as f32 -> f32 { return x as f32; }
```

---

## 枚举

```lumos
enum Color by i32 {          // as type（默认）：枚举作为类型
  Red = 0, Green = 1, Blue = 2,
}

enum Flags by u32 as container { // as container：元素是值，不是类型
  READ = 1, WRITE = 2, EXEC = 4,
}

enum HttpStatus by i32 as table { // as table：多列关联数据
  [enum,   i32, string          ],
  [OK,     200, "OK"            ],
  [NotFound, 404, "Not Found"   ],
}
// 用法：HttpStatus::NotFound as string  →  "Not Found"
```

---

## 模板与泛型

```lumos
// 有类型参数
def</T/> identity(T x) -> T = x;

// 模板特化
act[io.out]</T/> print_val(T x) { println(x); }
act[io.out] print_val</i32/>(i32 x) { println(`整数: $x`); }  // 特化

// 无类型参数（duck typing）
template add(a, b) { return a + b; }

// 类型约束
@staticassert(T is numtype)
def</typename T/> square(T x) -> T = x * x;
```

---

## 异常处理

```lumos
// 两种机制（正交）
// 1. 值级错误（无栈展开，def/fun/act 均可用）
fun divide(i32 a, i32 b) -> i32 or DivisionError {
  if (b == 0) return DivisionError("除零");
  return a / b;
}

// 2. 异常传播（需 act[exn]，有栈展开）
act[exn, fs.read] read_file(string path) -> string {
  val f = open(path) or throw FileNotFoundError(path);
  return f.read_all();
}

// 捕获
my_func() or (e as Error) { println(`error: $e`); }
my_func() or return -1;           // 简写
my_func() or;                     // 忽略（慎用）
try { f1(); f2(); } or (e) { ... } // try 块
throw "message";                   // 抛出（字符串自动转 Error）
```

---

## 模块系统

```lumos
use "std";                  // 导入到模块默认命名空间
use "std" -> std;           // 导入到 std 命名空间
use "std" -> ::;            // 导入到根命名空间
use std.println;            // 导入特定符号
```

**配置文件**（三选一，同目录只能有一个）：`lumos.yaml` / `lumos.json` / `lumos.toml`

```yaml
name: my_app
type: app          # app / lib / meta
dependencies: [std]
use: ["std -> ::"]
decl: "**.lh"
impl: "**.lm"
```

---

## 计量单位

```lumos
measure Length { m; km = 1000*m; cm = 0.01*m; }
measure Time   { s; ms = 0.001*s; min = 60*s; }

val dist = 100.0[m];          // 声明带单位变量
val speed: f64[m/s] = 10.0;
val total = 1.0[km] + 500.0[m];  // 自动换算 → 1500.0[m]
val t_ms = 1.5[s] in ms;         // 单位转换 → 1500.0[ms]

// 量纲别名
measure Velocity = Length / Time;
```

零运行时开销：单位在编译期类型检查后抹除。

---

## 词法规则

### 标识符

- ASCII 默认：`[a-zA-Z_][a-zA-Z0-9_]*`
- Unicode 标识符：`$"变量名"` 格式
- 命名约定：`snake_case`（变量/函数）、`PascalCase`（类型）、`UPPER_CASE`（常量）

### 字符串字面量

| 形式 | 说明 |
|------|------|
| `"..."` | 普通字符串（str，UTF-8） |
| `` `...` `` | 格式化字符串（`$var` / `${expr}` / `$expr%d`） |
| `'''...'''` | 原始字符串（无转义） |
| `"""..."""` | 多行字符串（`\|` 新行，`\+` 续行，`\*` 无空格续行） |
| `u8"..."` `u16"..."` | 显式编码字符串 |
| `b"..."` | 字节串 |
| `c"..."` | C 风格字符串（`\0` 结尾） |

转义：`\xNN`（2位十六进制，插入 byte）、`\uNNNN`（4位十六进制，插入字符）、`\[1f600]`（Unicode 码点）。

### 数字字面量

```lumos
123      123_456       // 十进制整数（下划线分隔）
0x1F2A   0b1010  0o17  // 十六/二/八进制
1.5      1.5e-3  1e10  // 浮点
123_i32  0xFF_u8        // 类型后缀
100.0[m]               // 带单位
```

---

## 标准库速查

| 模块 | 说明 |
|------|------|
| `std.io` | `print`/`println`/`printf`，`scan`/`scanln`，stdin/stdout/stderr |
| `std.string` | 字符串搜索、替换、分割、转换 |
| `std.memory` | 内存分配、智能指针（Rc/Arc/Box）、memset/memcpy |
| `std.container` | Array/List/HashMap/Set/Queue/Stack/PriorityQueue |
| `std.file` | 文件读写、目录操作、路径处理 |
| `std.thread` | 线程、Mutex、RWLock、Condvar、Channel、原子类型 |
| `std.random` | rand/srand、RNG、各类分布 |
| `std.algorithm` | 搜索/排序/变换/分区/数值算法 |

```lumos
// I/O
std.println("Hello, {}!", name);
i32 x = std.scan<i32>();

// 容器
var list = List::<i32>::new();
list.push(1);
val map = HashMap::<str, i32>::new();
map.set("key", 42);

// 随机
std.srand_auto();
i32 n = std.rand_range(1, 100);
```

---

## 文档导航

| 内容 | 路径 |
|------|------|
| 项目概览 | [`docs/overview.md`](docs/overview.md) |
| 语法速览（AI 参考）| [`docs/home/syntax.md`](docs/home/syntax.md) |
| 阅读路线 | [`docs/home/guide.md`](docs/home/guide.md) |
| 函数与纯度 | [`docs/grammar/function.md`](docs/grammar/function.md) |
| 变量声明 | [`docs/grammar/variable.md`](docs/grammar/variable.md) |
| 类型系统 | [`docs/grammar/type.md`](docs/grammar/type.md) |
| 控制流程 | [`docs/grammar/controlflow.md`](docs/grammar/controlflow.md) |
| 运算符 | [`docs/grammar/operation.md`](docs/grammar/operation.md) |
| 权限系统 | [`docs/grammar/permission.md`](docs/grammar/permission.md) |
| 权限清单 | [`docs/grammar/permissions.md`](docs/grammar/permissions.md) |
| 异常处理 | [`docs/grammar/exception.md`](docs/grammar/exception.md) |
| 模块系统 | [`docs/grammar/modules.md`](docs/grammar/modules.md) |
| 计量单位 | [`docs/grammar/measure.md`](docs/grammar/measure.md) |
| 关键字列表 | [`docs/lexical/keyword.md`](docs/lexical/keyword.md) |
| 词法规则 | [`docs/lexical/index.md`](docs/lexical/index.md) |
| 字符串字面量 | [`docs/lexical/string.md`](docs/lexical/string.md) |
| 标准库总览 | [`docs/std/index.md`](docs/std/index.md) |
| AST 结构 | [`docs/ast/index.md`](docs/ast/index.md) |
| IR 设计 | [`docs/ir/index.md`](docs/ir/index.md) |
