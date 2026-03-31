
# 控制流程 {#title}

关键字列表与用法速查请参阅 [语法/关键字](keyword.md#title)。

分支包含：

```lumos
if (条件表达式) {
    代码块
} elif (条件表达式) {
    代码块
} else {
    代码块
}

代码 if 条件表达式;

条件表达式 then 代码;
```

<span style="color:green">与 C 类似的部分不再展开说明</span>

## 分支 {#branches}

### 条件分支 `if` {#if}

`if` 语句是一种条件分支，根据条件执行不同的代码块。

```lumos
if (条件表达式) {
    代码块
}

if (条件表达式) 代码;
```

也可以后置 `if` 语句。

```lumos
代码 if 条件表达式;
```

`if` 中可以写多个表达式，并用 `,` 分隔；最后一个表达式作为条件。  
<span style="color:green">表达式中可以定义变量，其作用域为 `if` 语句内</span>

```lumos
if (表达式, 表达式, 条件表达式) {
    代码块
}
```

```lumos
if (var a = my_func(), a != null) return a;
// 等效于
if (var a = my_func()) return a;
```

### 条件分支 `else` {#else}

`else` 是可选分支，用于处理前面条件都不满足的情况。

```lumos
if (条件表达式) {
    代码块1
} else {
    代码块2
}
```

### `if` 表达式 {#if-expression}

`if` 可以作为表达式进行求值。作为表达式使用时必须携带 `else` 分支，两个分支的求值结果类型须相同（或可隐式转换）。

```lumos
i32 result = if (x > 0) x else -x;

str label = if (score >= 60) "pass" else "fail";
```

也可以配合 `elif`：

```lumos
str grade = if (score >= 90) "A" elif (score >= 60) "B" else "C";
```

### 条件分支 `then` {#then}

`then` 是一个二元算子，连接条件表达式与语句。当条件为真时执行后续语句，为假时跳过。

```lumos
条件表达式 then 代码;
```

```lumos
a > 0 then println("a is positive.");
```

### 多条件分支 `elif` {#elif}

`elif` 是可选分支，用于处理多个条件。

```lumos
if (条件表达式1) {
    代码块1
} elif (条件表达式2) {
    代码块2
} else {
    代码块3
}
```

### 多条件分支 `switch` {#switch}

`switch` 是一种多分支语句，根据目标表达式的值与各分支的常量进行相等比较，执行匹配的分支。  
语义上等价于 `if - elif - else` 序列，但写法更紧凑。  
<span style="color:green">由于现代编译器的优化，在编译期将 `switch` 展开为 `if` 序列不会导致性能下降</span>

```lumos
switch (表达式) {
    常量表达式1: {
        代码块1
    }
    常量表达式2: {
        代码块2
    }
    else: {
        代码块3
    }
}
```

`switch` 中的每个条目由“常量表达式 + `:` + 语句或代码块”组成。

!!! question "为什么不使用 `case`？"
    Lumos 不会像 C 那样把 `case` 仅作为跳转标签并依赖手动 `break`。在 Lumos 中，`switch` 内的条目本身就是条件分支，因此无需再引入与普通标签区分的 `case` 关键字。

`leave` 用于退出 `switch`。  
`fallthrough` 用于继续执行下一个分支。  
`else` 是可选分支，用于处理没有匹配项的情况。

```lumos
switch (num) {
    1: {
        for (var i in $[2, 3, 5, 7, 11]) {
            println(i);
        }
    } // 代码块结束，自动跳出
    2: fallthrough; // 向下跳
    3: fallthrough; // 向下跳
    4: {
        println("2 或 3 或 4");
    }
    else: leave; // 跳出 switch
}
```

任何数据类型只要重载 `==` 运算符就能用于 `switch`；若同时重载 `\hash` 运算符，还可启用匹配优化。

### 多条件分支 `match` {#match}

`match` 也是多分支语句，但与 `switch` 不同的是，它支持基于"模式"而非"相等值"的分支选择。  
换言之，`match` 可以进行更复杂的条件匹配（如模式解构等），而 `switch` 仅支持相等值比较。  
<span style="color:green">`match` 语句是 `switch` 语句的增强版</span>

```lumos
match (表达式) {
    匹配表达式1: {
        代码块1
    }
    匹配表达式2: {
        代码块2
    }
    else: {
        代码块3
    }
}
```

`match` 支持省略初始表达式。  
此时，各分支直接使用条件表达式进行判断，相当于 `if - elif - else`。

```lumos
match {
    条件表达式1: {
        代码块1
    }
    条件表达式2: {
        代码块2
    }
    else: {
        代码块3
    }
}
```

相当于：

```lumos
if (条件表达式1) {
    代码块1
} elif (条件表达式2) {
    代码块2
} else {
    代码块3
}
```

---

## 静态断言与假设 {#static-assertions}

### 假设条件成立 `assume` {#assume}

`assume` 用于向编译器提供优化假设。编译器可据此移除相关运行时检查或进行激进的代码优化。

```lumos
val a = get_input();
assume a != 0;            // 假设 a 不为零
val b = 100.0 / a as f32; // Release 模式下编译器将不再生成除零检查代码
```

**编译模式下的行为**：

- **Release 模式**：`assume` 纯粹作为优化提示，不生成任何运行时检查代码。若条件在实际运行时不成立，导致**未定义行为 (UB)**。
- **Debug 模式**：`assume` 自动转换为 `assert` 进行运行时校验。若条件不成立，程序会崩溃或抛出异常，便于及时发现问题。
- 可以使用编译标志显式禁用 Debug 模式中 `assume` 生成的 `assert`，此时行为与 Release 模式相同。

**混合编译**：

- 当项目包含多个编译单元时，每个编译单元按照自己的编译模式（Debug 或 Release）独立处理 `assume`。
- 若编译器能证明 `assume` 下的代码路径不可达，将产生编译错误。

---

## 循环 {#loops}

### 条件循环 `while` {#while}

`while` 是条件循环。仅当条件为真时循环体才会执行；每次迭代结束后重新检查条件。

```lumos
var i32 i = 0;
while (i < 10) {
    println(i);
    i += 1;
}
```

```lumos
while (条件表达式) {
    循环体
}
```

### 条件循环 `do while` {#do-while}

`do while` 也是条件循环。与 `while` 不同的是，循环体至少执行一次，然后才判断条件。

```lumos
var i32 i = 0;
do {
    println(i);
    i += 1;
} while (i < 10);
```

```lumos
do {
    循环体
} while (条件表达式);
```

### 计数循环 `for` {#for}

`for` 循环支持显式的初始化、条件检查和更新步骤，适合循环计数场景。

```lumos
for (var i32 i = 0; i < 10; i += 1) {
    println(i);
}
```

```lumos
for (初始化语句; 条件表达式; 更新语句) {
    循环体
}
```

当不需要显式计数器变量时，可以直接写成：

```lumos
for (迭代次数) {
    循环体
}
```

### 遍历循环 `for` {#loops-for}

`for` 循环支持对容器和范围的直接遍历，无需显式管理计数变量。

```lumos
for (val i in .[1, 2, 3, 4, 5]) {
    println(i);
}

for (val i in 1 .. 5) {
    println(i);
}
```

```lumos
for (变量 in 容器) {
    循环体
}
```

### 循环标签 {#loops-loops}

给循环添加标签后，可以快速跳出多层循环。*见 `break`*

```lumos
loop:
for (var i32 i = 0; i < 10; i += 1) {
    for (var i32 j = 0; j < 10; j += 1) {
        if (j == 5) break loop;
        println(j);
    }
}
```

### 无条件循环 {#loops-detail}

Lumos 支持显式的无条件循环（死循环）。无条件循环会持续执行，直到遇到 `break` 或 `leave` 才会退出。适用于需要特殊控制逻辑或持续运行的场景。

```lumos
loop {
    condition then break;  // 满足条件时退出
    do_something();
}
```

<span style="color:red">**注意**：`while(true)` 会导致编译错误，无条件循环必须使用 `loop` 关键字。</span>

### 循环属性 {#loops-attributes}

#### limit {#limit}

`@limit(次数)` 属性用于限制循环的最大执行次数，防止出现无限循环。达到指定次数后会自动执行 `break`。

```lumos
@limit(10)
for (var i32 i = 0; i < 100; i += 1) {
    println(i);
} broken {
    println("Limit reached.");
}
```

#### unroll {#unroll}

`@unroll(次数)` 属性可以展开循环，减少循环的开销。

`@unroll(0)` 或 `@unroll` 会将循环完全展开；`@unroll(次数)` 会按指定次数展开。这里的“次数”指展开后单次迭代中循环体的执行次数。

如果循环的次数不固定，则不能完全展开。

**属性组合**：`@limit` 与 `@unroll` 可同时使用，例如 `@limit(10) @unroll(3)`。

执行顺序为：

1. 先将循环体按因子 3 展开（每个展开的迭代单元中原循环体执行 3 次）
2. 再将最多执行 10 个展开后的迭代单元（相当于最多 30 次原循环体逻辑）

:::column

```lumos
@unroll
for (var i32 i = 0; i < 5; i += 1) {
    println(i);
}
```

:::flex-0

=>

:::

```lumos
println(0);
println(1);
println(2);
println(3);
println(4);
```

:::endcolumn

:::column

```lumos
@unroll(3)
for (var i32 i = 0; i < 10; i += 1) {
    println(i);
}
```

:::flex-0

=>

:::

```lumos
for (var i32 i = 0; i < 10; i += 3) {
    println(i);
    if (i + 1 >= 10) break;
    println(i + 1);
    if (i + 2 >= 10) break;
    println(i + 2);
}
```

:::endcolumn

## 跳转 {#jumps}

### 跳过本次循环 `continue` {#continue}

`continue` 用于结束本次循环并进入下一次循环。

有等效的 `goto` 语句：

```lumos
for (var i32 i = 0; i < 10; i += 1) {
    if (i % 2 == 0) continue;
    println(i);
}

for (var i32 i = 0; i < 10; i += 1) {
    if (i % 2 == 0) goto next;
    println(i);
  next:
}
```

### 跳出循环 `break` {#break}

`break` 用于立即跳出循环。

有等效的 `goto` 语句：

```lumos
for (var i32 i = 0; i < 10; i += 1) {
    if (i == 5) break;
    println(i);
}

for (var i32 i = 0; i < 10; i += 1) {
    if (i == 5) goto end;
    println(i);
}
end:
```

### 跳出多层循环 {#jumps-loops}

可以为循环添加标签，然后在 `break` 语句中指定标签来跳出多层循环。`break` 支持跳出任意数量的嵌套层级。

```lumos
outer:
for (var i32 i = 0; i < 10; i += 1) {
    inner:
    for (var i32 j = 0; j < 10; j += 1) {
        if (i == 5 and j == 5) break outer;  // 可以跳过 inner，直接跳出 outer
        println(i, j);
    }
}
```

### 跳出代码块 `leave` {#leave}

`leave` 用于提前退出代码块。  
<span style="color:purple">必须是使用 `{}` 包裹的代码块。下面的示例语句不算是代码块。</span>

`leave` 可退出任何 `{}` 包裹的代码块，包括 `if`、`switch` 等的代码块。

```lumos
if (表达式) leave;
```

使用示例：

```lumos
{
    i32 a = 1;
    if (a == 1) leave;
    println("Hello, World!");
}   // 不会打印文本
```

对于可求值代码块，`leave` 后可指定返回值。<span style="color:green">语义类似函数中的 `return`</span>

```lumos
let a = val {
    i32 b = 1;
    if (b == 1) leave 0;
    b; // 不会执行到此处
}
println(a); // 0
```

作为函数体的代码块只能使用 `return`，不能使用 `leave`。

```lumos
act[io.out] my_func(i32 a) -> unit {
    if (a == 1) leave; // 会报错，请使用 return
    println("Hello, World!");
}
```

## 跳出后 {#after-exit}

### 当循环被中断时 `broken` {#broken}

`broken` 用于在循环被 `break` 中断时执行代码。

它可以减少某些场景下的深层嵌套。

```lumos
for (var i32 i = 0; i < 10; i += 1) {
    if (i == 5) break;
    println(i);
} broken {
    println(`Loop broken with i = $i.`);
}
```

可以在 `broken` 中使用 `continue`，这将进入循环的下一次迭代（而非重新进入被中断的循环）。

```lumos
for (var i32 i = 0; i < 10; i += 1) {
    if (i == 5) break;
    println(i);
} broken {
    println(`Loop broken with i = $i.`);
    continue;  // 进入下一次迭代
}
```

### 当循环正常结束时 `completed` {#after-exit-completed}

`completed` 用于在循环正常结束后执行代码（即循环体执行完毕且未被 `break` 中断）。

```lumos
for (var i32 i = 0; i < 10; i += 1) {
    println(i);
} completed { // 会执行
    println(`Loop finished with i = $i.`);
}
```

```lumos
for (var i32 i = 0; i < 10; i += 1) {
    if (i == 5) break;
    println(i);
} completed { // 不会执行
    println(`Loop finished with i = $i.`);
}
```

<span style="color:green">`broken` 与 `completed` 可同时使用。两个分支中都能访问循环的迭代变量</span>

## 跳转 {#jumps-detail}

### 标签跳转 `goto` {#jump-to-label}

`goto` 用于无条件跳转到指定标签。标签通常使用反向缩进（减少两格）来标示层级。

```lumos
  loop:
    println("Hello world!");
    goto loop;
```

使用标签跳转时，不能跳过变量初始化语句。

```lumos
    if (某些条件) goto label; // 非法：跳转到 label 后无法确定 my_var 的值。
    val my_var as usize = 10;
    println(my_var);
  label:
    println("Hello world!");
```

### 任意地址跳转 `goto` {#jump-to-address}

`goto` 也可用于跳转到任意地址，对应汇编中的 `jmp` 指令。跳转目标可以是编译期常数地址，也可以是变量存储的地址。

```lumos
// 直接地址
goto 0x12345678;

// 或通过变量
val [unit] addr = ...;  // 从某处获取地址
goto addr;
```

对标签取地址后为其后一行代码地址，类型为 `[unit]`。

```lumos
  loop:
    println("Hello world!");
    val [unit] addr = &loop;
    goto addr;
```

---

相关内容：异常处理见 [异常处理](exception.md#title)。
