
# 控制流程

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

<span style="color:green">与 C 类似的部分，不会过多赘述</span>

## 分支

### 条件分支 `if`

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

可以在 `if` 中写多个表达式，用 `,` 隔开。条件表达式为最后一个表达式。<br />
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

### 条件分支 `else`

`else` 语句是一个可选的标签，用于处理没有匹配的情况。

```lumos
if (条件表达式) {
    代码块1
} else {
    代码块2
}
```

### 条件分支 `then`

`then` 用于表达式和语句之间，在表达式为真时执行语句。

```lumos
条件表达式 then 代码;
```

```lumos
a > 0 then println("a is positive.");
```

### 多条件分支 `elif`

`elif` 语句是一个可选的标签，用于处理多个条件的情况。

```lumos
if (条件表达式1) {
    代码块1
} elif (条件表达式2) {
    代码块2
} else {
    代码块3
}
```

### 多条件分支 `switch`

`switch` 语句是一种多条件分支，根据条件执行不同的代码块。<br />
实际上的实现就是 `if - elif - else` 序列，只是更加简洁。<br />
<span style="color:green">由于现代编译器的优化，在编译时将 `switch` 展开为 `if` 并不会导致低性能</span>

```lumos
switch (表达式) {
    常量表达式1: {
        代码块1
    }
    常量表达式2: {
        代码块2
    }
    @default: {
        代码块3
    }
}
```

`switch` 中的每个条目由 常量表达式 + `:` + 语句或代码块 组成。

!!! question "为什么不使用 `case`？"
    Lumos 不会像 C 语言那样只把 `case` 处当作跳转的标签（要和普通标签区分开），需要在下一段代码前手动 `break`，所以不需要和普通标签区分，直接出现在 `switch` 内的必定是条件表达式。

`leave` 语句用于跳出 `switch` 语句。<br />
`@next` 语句用于跳到下一个条件的处理过程。<br />
`@default` 是一个可选的标签，用于处理没有匹配的情况。

> 不是？你属性咋跑这来了

```lumos
switch (num) {
    1: {
        for (var i in [2, 3, 5, 7, 11]) {
            println(i);
        }
    } // 代码块结束，自动跳出
    2: @next; // 向下跳
    3: @next; // 向下跳
    4: {
        println("2 或 3 或 4");
    }
    @default: leave; // 跳出 switch
}
```

任何数据类型，只要重载 `==` 运算符就能支持 `switch`，只要重载 `\hash` 运算符就能进行匹配优化。

### 多条件分支 `match`

与 `switch` 类似，`match` 语句是一种多条件分支，根据条件执行不同的代码块，但条件为匹配表达式而不是匹配值。<br />
<span style="color:green">`match` 语句是 `switch` 语句的增强版</span>

```lumos
match (表达式) {
    匹配表达式1: {
        代码块1
    }
    匹配表达式2: {
        代码块2
    }
    @default: {
        代码块3
    }
}
```

无初始表达式的 `match` 语句。<br />
可以当作另一种 `if - elif - else` 使用。

```lumos
match {
    条件表达式1: {
        代码块1
    }
    条件表达式2: {
        代码块2
    }
    @default: {
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

## 循环

### 条件循环 `while`

`while` 循环是一种条件循环，只要条件为真，循环就会一直执行。

```lumos
int i = 0;
while (i < 10) {
    println(i);
    i++;
}
```

```lumos
while (条件表达式) {
    循环体
}
```

### 条件循环 `do while`

`do while` 循环是一种条件循环，先执行循环体，再判断条件。

```lumos
int i = 0;
do {
    println(i);
    i++;
} while (i < 10);
```

```lumos
do {
    循环体
} while (条件表达式);
```

### 计数循环 `for`

`for` 循环是一种计数循环，可以在循环体内使用计数器。

```lumos
for (int i = 0; i < 10; i++) {
    println(i);
}
```

```lumos
for (初始化语句; 条件表达式; 更新语句) {
    循环体
}
```

当我们不需要计数器变量时可以直接：

```lumos
for (迭代次数) {
    循环体
}
```

### 遍历循环 `for`

`for` 循环是一种遍历循环，可以在循环体内遍历容器。

```lumos
for (val i : [1, 2, 3, 4, 5]) {
    println(i);
}

for (val i : 1 .. 5) {
    println(i);
}
```

```lumos
for (变量 : 容器) {
    循环体
}
```

### 循环标签

给循环添加标签可以用于快速跳出多层循环。*见 `break`*

```lumos
loop:
for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
        if (j == 5) break loop;
        println(j);
    }
}
```

### 关于死循环

Lumos 中允许死循环的存在，如果你想用死循环实现一些奇怪的控制逻辑，那么是可以的。

### 循环属性

#### limit

`@limit(次数)` 属性可以限制循环的最大执行次数，防止出现无限循环，其实现为循环达到指定次数后自动调用 `break`。

```lumos
@limit(10)
for (int i = 0; i < 100; i++) {
    println(i);
} breaked {
    println("Limit reached.");
}
```

#### unroll

`LMX` ::

`@unroll(次数)` 属性可以展开循环，减少循环的开销。<br />
`@unroll(0)` 或 `@unroll` 属性将循环完全展开，`@unroll(次数)` 属性将循环展开指定次数。<br />
次数指展开后一次循环中循环体的执行次数。

:::column

```lumos
@unroll
for (int i = 0; i < 5; i++) {
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
for (int i = 0; i < 10; i++) {
    println(i);
}
```

:::flex-0

=>

:::

```lumos
for (int i = 0; i < 10; i += 3) {
    println(i);
    if (i + 1 >= 10) break;
    println(i + 1);
    if (i + 2 >= 10) break;
    println(i + 2);
}
```

:::endcolumn

## 跳转

### 跳过本次循环 `continue`

`continue` 用于跳过本次循环，跳到下一次循环。

有等效的 `goto` 语句：

```lumos
for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) continue;
    println(i);
}

for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) goto next;
    println(i);
  next:
}
```

### 跳出循环 `break`

`break` 用于跳出循环。

有等效的 `goto` 语句：

```lumos
for (int i = 0; i < 10; i++) {
    if (i == 5) break;
    println(i);
}

for (int i = 0; i < 10; i++) {
    if (i == 5) goto end;
    println(i);
}
end:
```

### 跳出多层循环

利用 `break` 加循环标签可以快速跳出多层循环。

```lumos
loop:
for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
        if (i == 5 && j == 5) break loop;
        println(i, j);
    }
}
```

### 跳出代码块 `leave`

`leave` 用于跳出代码块。<br />
<span style="color:purple">必须是使用 `{}` 包裹的代码块。下面的示例语句不算是代码块。</span>

`leave` 也可以跳出 `if` `switch` 的代码块。

```lumos
if (表达式) leave;
```

使用示例：

```lumos
{
    int a = 1;
    if (a == 1) leave;
    println("Hello, World!");
}   // 不会打印文本
```

如果是可求值代码块，`leave` 后跟代码块的求值表达式。<span style="color:green">类似 `return`</span>

```lumos
let a = val {
    int b = 1;
    if (b == 1) leave 0;
    b; // 不会执行到此处
}
println(a); // 0
```

作为函数体的代码块只能 `return` 而不能 `leave`。

```lumos
fn my_func(int a) {
    if (a == 1) leave; // 会报错，请使用 return
    println("Hello, World!");
}
```

## 跳出后

### 当循环被中断时 `breaked`

!!! question ""
    不过有人提议使用 `terminated`

`breaked` 用于在循环被 `break` 中断时执行代码。<br />
<span style="color:green">减少逆天的嵌套</span>

```lumos
for (int i = 0; i < 10; i++) {
    if (i == 5) break;
    println(i);
} breaked {
    println(`Loop breaked with i = $i.`);
}
```

可以在 `breaked` 中 `continue` 重新进入循环。<br />
<span style="color:green">本次循环会被跳过</span>

```lumos
for (int i = 0; i < 10; i++) {
    if (i == 5) break;
    println(i);
} breaked {
    println(`Loop breaked with i = $i.`);
    continue;
}
```

### 当循环正常结束时 `then`

`then` 用于在循环正常结束时执行代码。

```lumos
for (int i = 0; i < 10; i++) {
    println(i);
} then { // 会执行
    println(`Loop finished with i = $i.`);
}
```

```lumos
for (int i = 0; i < 10; i++) {
    if (i == 5) break;
    println(i);
} then { // 不会执行
    println(`Loop finished with i = $i.`);
}
```

<span style="color:green">以上两个可以同时使用</span>

<span style="color:green">`breaked` 和 `then` 中都可以使用迭代变量</span>

## 跳转

### 标签跳转 `goto`

`goto` 用于跳转到指定标签。<br />
<span style="color:green">所有标签反向缩进两格</span>

```lumos
  loop:
    println("Hello world!");
    goto loop;
```

对于标签跳转，无法跳过变量的初始化。

```lumos
    if (某些条件) goto label; // 这是不行的，因为跳转到 label 后无法确定 my_var 的值。
    val my_var as usize = 10;
    println(my_var);
  label:
    println("Hello world!");
```

### 任意地址跳转 `goto`

`goto` 用于跳转到任意地址，对应汇编中的 jmp 指令。

```lumos
goto 0x12345678;
```

对标签取地址后为其后一行代码地址，类型为 `void*`。

```lumos
  loop:
    println("Hello world!");
    let addr = &loop;
    goto addr;
```
