
# 变量

## 变量声明

Lumos 提供了四种不同的变量修饰符，用于精确控制变量的可变性和内存行为。

### 1. 变量声明 `var` (Variable - 完全可变)

* **语义**：变量名可以重新绑定（除非有 `fin`），内存内容可以随意修改。
* **隐含修饰**：`var` 隐含了类型修饰符 `mut`，手动重复书写 `var mut` 会导致编译错误。
* **默认访问**：默认为 `rw` (Read-Write)。

```lumos
var i32 a = 1;
a = 2;    // 允许重新绑定
a[] = 3;  // 允许修改内存内容
```

### 2. 变量声明 `val` 类型修饰默认 (Value - 逻辑不可变)

* **语义**：变量名绑定后不可更改。对外表现的“值”是稳定的，但允许**内部可变性**（Internal Mutability）。
* **内存**：底层内存可能会变（例如：懒加载初始化、引用计数增减、内部缓存更新）。
* **简写**：`val T` 可以被简写为 `T`。

```lumos
val i32 b = 1;
i32 c = 2;     // 等同于 val i32 c = 2
b = 2;         // error: 变量名绑定后不可更改
```

### 3. 变量声明 `imv` (Immutable - 物理不可变)

* **语义**：变量名不可更改，且**内存表示（Bit Pattern）**在初始化后绝对锁定。
* **隐含修饰**：`imv` 隐含了类型修饰符 `imm`，手动重复书写 `imv imm` 会导致编译错误。
* **内存**：编译器可以安全地将其放入只读内存段（.rodata），不允许任何字节级别的修改。

```lumos
imv i32 d = 1;
d = 2;    // error
d[] = 2;  // error: 物理内存不可修改
```

### 4. 变量声明 `lit` 不能修饰类型 (Literal - 编译期常量)

* **语义**：仅存在于编译阶段。
* **内存**：不占用运行时内存地址，在所有使用处进行常量折叠或内联替换。

```lumos
lit i32 e = 1;
```

---

**注意**：逻辑不可变（`val`）或物理不可变（`imv`）变量不能被隐式默认初始化，必须在声明时或构造函数中明确初始化。

---

## 延迟初始化

如果一个变量无法在声明时立即初始化，Lumos 提供了 `late` 和 `lazy` 关键字。

### 1. 手动延迟初始化 `late`

使用 `late` 标记一个变量将在稍后手动初始化。在初始化之前访问该变量会导致运行时错误。

```lumos
val GLOBAL = late i32;

act init() {
    GLOBAL = 42; // 手动初始化
}
```

### 2. 懒加载初始化 `lazy`

使用 `lazy` 标记一个变量在第一次被访问时才进行初始化。初始化过程是线程安全的。

```lumos
// 自动推断类型
val GLOBAL = lazy {
    println("Initializing...");
    42
};

// 显式指定类型
val GLOBAL_TYPE = lazy i32 {
    42
};
```

---

## 访问修饰符

对于逻辑的读写操作，Lumos 增加了四种类型修饰符：

* `ro` (Read-Only)：只能读，不能写。
* `wo` (Write-Only)：只能写，不能读。
* `rw` (Read-Write)：可读可写。
* `rx` (Read-Execute)：可读可执行，不能写。

`var` 默认为 `rw` 但可以手动标记为别的属性。

### 在类型中使用

```lumos
act my_func([wo i32] data) -> void {
    data[] = 123; // 允许写
    i32 value = data[]; // error: 不允许读
}
```

### 在成员函数中使用

在成员函数中，可以使用 `@` 符号标记函数对对象成员的访问权限：

```lumos
fun@ro my_func() -> i32 {
    // 只能读成员变量，不能写
}
```

---

### 禁止重新绑定 `fin`

使用 `fin` 关键字声明的变量禁止在同一作用域内被重新绑定（Shadowing）。

```lumos
fin a = 1;
val a = a + 1; // error: 变量名 `a` 已被 `fin` 锁定，不能重新绑定
```

---

### 声明多个变量

声明多个同类型变量：

```lumos
i32 a, b, c;
var i32 d, e, f;
```

注意：在 Lumos 中，未初始化的变量在使用前必须被赋值，否则会导致编译错误。

声明多个不同类型变量：

```lumos
i32 a = 1; f32 b = 2.0;
```

---

### 指针与引用声明

声明指向逻辑不可变内存的逻辑不可变指针：

```lumos
[i32] a = 0x12345678;
a = null;    // error: 无法将 null 赋值给逻辑不可变变量
a[] = 1;     // error: 无法修改逻辑不可变内存
```

声明指向完全可变内存的完全可变指针：

```lumos
var [var i32] b = 0x12345678;
b = null;    // success
b[] = 1;     // success
```

---

### 非空指针与引用

非空指针使用 `&` 修饰：

```lumos
&i32 a = 0x12345678;
&i32 b = null; // error: 无法将 null 赋值给非空指针
```

唯一引用使用 `!` 修饰：

```lumos
!i32 a = 0x12345678;
```

---

### 类型断言

可以在类型后添加断言条件：

```lumos
i32 a ($ > 0) = 1;
var i32 b ($ > 0) = 0; // error: 无法将 0 赋值给 i32($ > 0)
```

此处 `$` 代表当前值，每次写入时都会进行校验。

---

***默认的规则***

对于一个变量，如果没有进行任何形式的取地址，那么它绝对不会被外部意外修改。

```lumos
var a = 1;
while (a > 0) {
  // Do nothing
}
```

这应当是一个死循环。

---

***允许的优化***

对于一个变量，取到的值可以是：

* 编译时能够确定的值（非 `volatile`）<br />
  *<span style="color:orange">warning</span>: <span style="color:darkcyan">对于编译期可确定的值使用 `lit`</span>*
* 上次修改后缓存的值（非 `volatile`）
* 对应内存地址当前的值

## 不可变变量声明 `val`

使用 `val` 关键字声明不可变变量，使用方法与 `var` 相同。

```lumos
val 变量名 = 初始化表达式;
val a = 1;
```

---

常量设计上在整个生命周期中不可变，但实际上可以强行修改，但进行修改时可能会因编译器的优化策略导致不可预测的结果。

```lumos
val a = 1;     // 定义常量
println(a);    // 输出 1
(&a as [i32])[] = 2; // 强行修改为 2
println(a);    // 由于编译器的优化策略，可能输出 1 或 2
```

??<span style="color:purple">这么做的见一个打一个</span>??

***允许的优化***

对于一个常量，取到的值可以是：

* 编译时能够确定的值（非 `volatile`）<br />
  *<span style="color:orange">warning</span>: <span style="color:darkcyan">对于编译期可确定的值使用 `lit`</span>*
* 作用域内任意位置缓存的值（非 `volatile`）
* 对应内存地址当前的值

## 初始化

Lumos 允许的初始化方式有：

* 赋值初始化 `i32 my_var = 1;`
* 构造函数初始化 `i32 my_var(1);`<br />
  <span style="color:green">对于基本数据类型有伪构造函数</span>
* 结构体元素赋值初始化 `MyStructure my_var = {1, 2, .third = 3};`
* 数组元素赋值初始化 `[3]i32 my_var = [1, 2, 3];`
* 默认初始化 `i32 my_var;`<br />
  <span style="color:green">默认初始化会将基本数据类型变量初始化为二进制 0，对于其它数据类型则调用默认构造函数</span>
  <span style="color:green">我们推荐显式初始化</span>

<!-- <span style="color:purple">注意 `MyStructure my_var{1, 2, 3};` 这样的 C++ 风格是不允许的</span>（见 getter setter） -->

默认初始化使得所有变量都会被初始化，即使没有给定初始化表达式。

```lumos
i32 a;    // 初始化为 0
f64 b;    // 初始化为 0.0
bool c;   // 初始化为 false
char d;   // 初始化为 '\0'
string e; // 初始化为 ""
```

变量类型与表达式类型不同时变量初始化被视为显式类型转换。但之后的赋值只允许隐式类型转换。

```lumos
[i32] a = 0x123456;
```

### 延迟初始化

使用 `lateinit` 作为初始值来让变量不自动初始化。<br />
<span style="color:green">注意访问未初始化的变量是未定义行为</span>

```lumos
i32 a = lateinit; // 此时 a 未初始化
a = 1;            // 手动初始化 a
println(a);       // 1
```

对于不可变变量，使用 `lateinit` 时仅可以赋值一次。

```lumos
i32 a = lateinit; // 此时 a 未初始化
a = 1;            // 手动初始化 a
println(a);       // 1
// a = 2;         // error: 无法重新赋值给不可变变量
```

```lumos
i32 a, b, c = lateinit;
if (xxx) {
  a = 1;
  b = 2;
  c = 3;
} else {
  a = 4;
  b = 5;
  c = 6;
}
```

### 懒初始化

使用 `lateinit` 接代码块作为初始值来让变量在第一次访问时自动初始化。<br />
代码块将延迟到第一次访问时执行，且只会执行一次。<br />
<span style="color:green">注意这种情况下不能连续声明多个变量</span>

```lumos
var i32 a = lateinit {
  return 1;
}; /* 此处语句已结束，不能接着声明变量 */
println(a); // 此时 a 被初始化为 1
```

代码块中可以使用外部的变量，但注意变量的值为代码块执行时的值，而非声明时的值。

```lumos
var i32 a = 1;
var i32 b = lateinit {
  return a;
};
a = 2;
println(b); // 输出 2
a = 3;
println(b); // 输出 2
```

## 全局不可变变量

你不应该声明一个全局的不可变变量，应该用 `let`(表达式) 或 `lit`(常量表达式) 代替。

见 [表达式](expression.md)。

## 临时变量 `with`

使用 `with` 关键字声明临时变量。

```lumos
with 变量名 = 初始化表达式; {
  // 此处可以引用临时变量
}

with 变量名 = 初始化表达式;
if 条件表达式 {
  // 此处可以引用临时变量
}

with 变量名 = 初始化表达式;
if 条件表达式 {
  // 此处可以引用临时变量
} else {
  // 此处可以引用临时变量
}
```

临时变量的作用域仅限于下一个表达式或代码块内，表达式或代码块结束后临时变量会被销毁。

`with` 后可以接 `val` 或 `var` 关键字来声明不可变或可变的临时变量。

```lumos
with var a = 1; {
  println(a); // 输出 1
}

## 限定符

### `restrict`

<span style="color:green">注意 `restrict` 不是属性</span>

`restrict` 限定符用于指针，表示指针所指向的内存区域不会被其他指针访问。

```lumos
[i32] restrict a = malloc(4);
[i32] restrict b = a; // 这是不可以的
[i32]          c = a; // 这也是不可以的
```

### `volatile`

<span style="color:green">注意 `volatile` 不是属性</span>

`volatile` 限定符用于变量，表示变量可能会被其它线程或硬件改变，编译器不会对其访问（读写）进行优化。

```lumos
volatile i32 a = 1;
```

## 属性

### `register`

`@register(寄存器名)` 属性用于强制变量存储在寄存器中，而不是内存中。<br />
<span style="color:green">这会导致相应寄存器无法被其它变量使用</span><br />
<span style="color:purple">无特殊需求不应该使用</span>

```lumos
@register("rax")
var i32 a = 1;
```

注意此处的 `rax` 填写 `eax` `ax` `al` 都是可以的，实际宽度由类型决定。

## `isrestrict` 运算符

`isrestrict` 运算符用于判断两个指针是否独立。

```lumos
[i32] a = malloc(4);
[i32] b = malloc(4);
[i32] c = a;

// 当编译器可以自动推断内存块大小时
println(isrestrict(a, b)); // 输出 true
println(isrestrict(a, c)); // 输出 false
// 当编译器不能自动推断内存块大小时
println(isrestrict(a, 4, b, 4)); // 输出 true
println(isrestrict(a, 4, c, 4)); // 输出 false
```

## getter/setter

你可以使用 getter 和 setter 来访问变量。

```lumos
var a by i32 {
  \get -> i32 { // 只能有一个 \get 但可以有多个 \set
    return 1;
  }
  \set(value as i32) -> void { // 不写参数默认为 value
    println(`set a to $value`);
  }
}

a = 2;      // 输出 set a to 2
println(a); // 输出 1
```

可以不声明类型：

```lumos
var a by {
  \get -> i32 {
    return 1;
  }
  \set(value as i32) -> void {
    println(`set a to $value`);
  }
}
```

可以在内部声明变量来存储值：

```lumos
var a by i32 {
  var i32 real_a = 1;
  \get -> i32 {
    return real_a;
  }
  \set(value as i32) -> void {
    println(`set a to $value`);
    real_a = value;
  }
}

a = 2;      // 输出 set a to 2
println(a); // 输出 2
```

外部无法访问到 `real_a` 变量。
