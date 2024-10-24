
# 运算、运算符

!!! question "为什么不使用 `operator` 这样的关键字？"

    我认为 `fn \add(MyInt rhs) -> MyInt` 这样的写法与 `auto operator+(MyInt rhs) -> MyInt` 这样的写法相比更加简洁，也容易理解。

优先顺序：

1. 后缀
2. 前缀
3. 二元

## 前缀运算

| 运算 | 名称  | 解释     | 注释     |
|------|-------|----------|----------|
| +a   | pos   | 正号     |          |
| -a   | neg   | 负号     |          |
| ~a   | bnot  | 按位取反 |          |
| !a   | not   | 逻辑取反 |          |
| ++a  | linc  | 前缀自增 |          |
| --a  | ldec  | 前缀自减 |          |
| *a   | deref | 解引用   |          |
| &a   | addr  | 取地址   | 不可重载 |

## 后缀运算

| 运算 | 名称  | 解释     |
|------|-------|----------|
| a++  | rinc  | 后缀自增 |
| a--  | rdec  | 后缀自减 |
| a[b] | index | 数组索引 |

如果重载索引：

```lumos
fn \index(usize i) -> int {
    return *(array + i);
}
```

## 二元运算

### 复合赋值运算符

| 运算    | 名称  | 解释           |
|---------|-------|----------------|
| a += b  | iadd  | 加并赋值       |
| a -= b  | isub  | 减并赋值       |
| a *= b  | imul  | 乘并赋值       |
| a /= b  | idiv  | 除并赋值       |
| a %= b  | imod  | 取模并赋值     |
| a &= b  | iband | 按位与并赋值   |
| a \|= b | ibor  | 按位或并赋值   |
| a ^= b  | ibxor | 按位异或并赋值 |
| a <<= b | ishl  | 左移并赋值     |
| a >>= b | ishr  | 右移并赋值     |

## 其他运算符

| 运算      | 名称    | 解释         |
|-----------|---------|--------------|
| a ? b : c | ternary | 三元运算符   |
| a, b      | comma   | 逗号运算符   |
| (type)a   | cast    | 类型转换     |
| sizeof(a) | sizeof  | 计算大小     |
| a->b      | arrow   | 指针成员访问 |
| a.b       | dot     | 成员访问     |

## 位运算符

注意浮点数不能进行位运算，如果你实在需要位运算，请将浮点数转换为 `bits` 类型，而不是像 C 中那样使用整数指针。

```lumos
f32 value = (b32)123.456 << 1;
// 而不是
f32  value = 123.456;
u32* ptr   = &value;
*ptr     <<= 1;
```

##

- 交换律：`a + b` 与 `b + a` 等效
- 结合律：`(a + b) + c` 与 `a + (b + c)` 等效

## 算术运算符

我们约定以下运算符等效，其互换不应影响程序的行为

```lumos
a = a 运算符 b;
a 运算符= b;
```

`a = a + b` 与 `a += b`<br>
`a = a - b` 与 `a -= b`<br>
`a = a * b` 与 `a *= b`<br>
`a = a / b` 与 `a /= b`<br>
*依此类推*

### 自增、自减

`a++` 与 `a += 1`<br>
`a--` 与 `a -= 1`<br>

`a if expr else b` 与 `expr ? a : b`<br>
`a and b` 与 `a && b`<br>
`a or b` 与 `a || b`<br>
`not a` 与 `!a`<br>

## 逻辑运算符

`&&` `||` `^^` 符合交换律，即：

`a && b` 与 `b && a` 等效<br>
`a || b` 与 `b || a` 等效<br>
*依此类推*

## 重载运算符

与 C++ 中 `opeator+` 一类的写法不同，Lumos 使用反斜杠加运算名来代表运算符，例如 `\add`。

| 运算  | 名称 | 运算   | 名称 | 运算     | 名称  |
|-------|------|--------|------|----------|-------|
| a + b | add  | a & b  | band | a && b   | and   |
| a - b | sub  | a \| b | bor  | a \|\| b | or    |
| a * b | mul  | a ^ b  | bxor | a ^^ b   | xor   |
| a / b | div  | a << b | shl  | a == b   | eq    |
| a % b | mod  | a >> b | shr  | a != b   | ne    |
| -a    | neg  | ~a     | bnot | !a       | not   |
| a     | get  | a < b  | lt   | a <= b   | le    |
| a = b | set  | a > b  | gt   | a >= b   | ge    |
| a++   | rinc | a--    | rdec | a <=> b  | cmp   |
| ++a   | linc | --a    | ldec | *a       | deref |

所有类似 `a += b` 的运算名称都是 `i` 加上对应的运算符名称，例如 `add` 的增量运算符是 `iadd`。

## `\` 开头的运算符

有些运算符并没有对应的符号版本，但也非常重要，这些运算符以 `\` 开头。

!!! question "为什么不使用特定名称的函数？"

    我认为不应该出现语法或优化和一些非标准库的特定名称函数有关的情况，所以我将它们定义为运算符。

- `\hash`
- `\next`
- `\prev`
