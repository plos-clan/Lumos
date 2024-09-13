
# 循环

## 条件循环 `while`

<span style="color:green">与 C 类似，不过多赘述</span>

`while` 循环是一种条件循环，只要条件为真，循环就会一直执行。

```lumos
int i = 0;
while (i < 10) {
    println(i);
    i++;
}
```

## 条件循环 `do while`

<span style="color:green">与 C 类似，不过多赘述</span>

`do while` 循环是一种条件循环，先执行循环体，再判断条件。

```lumos
int i = 0;
do {
    println(i);
    i++;
} while (i < 10);
```

## 计数循环 `for`

<span style="color:green">与 C 类似，不过多赘述</span>

`for` 循环是一种计数循环，可以在循环体内使用计数器。

```lumos
for (int i = 0; i < 10; i++) {
    println(i);
}
```

## 循环标签

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

## 循环控制

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

## 在循环之后继续执行代码

### 当循环被中断时 `breaked`

`breaked` 用于在循环被 `break` 中断时执行代码。<br>
<span style="color:green">减少逆天的嵌套</span>

```lumos
for (int i = 0; i < 10; i++) {
    if (i == 5) break;
    println(i);
} breaked {
    println("Loop breaked");
}
```

```lumos
for (int i = 0; i < 10; i++) {
    break if i == 5;
    println(i);
} breaked {
    println("Loop breaked");
}
```

可以在 `breaked` 中 `continue` 重新进入循环。<br>
<span style="color:green">本次循环会被跳过</span>

```lumos
for (int i = 0; i < 10; i++) {
    if (i == 5) break;
    println(i);
} breaked {
    println("Loop breaked");
    continue;
}
```

### 当循环正常结束时 `then`

`then` 用于在循环正常结束时执行代码。

```lumos
for (int i = 0; i < 10; i++) {
    println(i);
} then {
    println("Loop finished");
}
```

<span style="color:green">以上两个可以同时使用</span>

<span style="color:green">`breaked` 和 `then` 中都可以使用迭代变量</span>