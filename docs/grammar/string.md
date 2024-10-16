# 字符串字面量、字符字面量

开头和结尾

- `'` 字符字面量
- `"` 字符串字面量
- `` ` `` 格式化字符串字面量
- `'''` 原始字符串字面量
- `"""` 多行字符串字面量
- ` ``` ` 多行格式化字符串字面量

## 反斜杠转义

使用反斜杠进行转义，转义序列忽略大小写<br>
<span style="color:green">除原始字符串字面量外都可以使用转义</span>

| 原始 | 字符 | 八进制 | 十六进制 | 解释         |
|------|------|--------|----------|--------------|
| `\0` | NUL  | 000    | 00       | 字符串结束   |
| `\a` | BEL  | 007    | 07       | 响铃         |
| `\b` | BS   | 010    | 08       | 退格         |
| `\e` | ESC  | 033    | 1b       | 转义         |
| `\f` | FF   | 014    | 0c       | 换页         |
| `\n` | LF   | 012    | 0a       | 换行         |
| `\r` | CR   | 015    | 0d       | 回车         |
| `\t` | TAB  | 011    | 09       | 水平制表符   |
| `\v` | VT   | 013    | 0b       | 垂直制表符   |
| `\x` | -    | -      | -        | 十六进制转义 |
| `\u` | -    | -      | -        | Unicode 转义 |

- `\000` 八进制转义 (三位八进制数，可省略前导0)<br>
  如：`\0` 表示空字符，`\033` 表示 ESC
- `\x00` 十六进制转义 (两位十六进制数)<br>
  如：`\x20` 表示空格，`\x1b` 表示 ESC
- `\u0000` Unicode 转义 (一至六位十六进制数，可省略前导0)<br>
  如：`\u0020` 表示空格，`\u001b` 表示 ESC

`\` 加其它字符则忽略 `\` 本身，如 `\'` 表示 `'` 字符

## 格式化表达式

使用类似 shell 的格式化写法，使用 `$变量名` `${表达式}` `${表达式@格式修饰符}` 三种形式。<br>
与其它情况下大括号表示代码块不同，此处大括号内为单个表达式，表达式可以换行。

```lumos
`$a + $b = ${a + b}`
`$a + $b = ${
  a + b
}`
`Pointer ptr is: ${ptr@p}`
```

要在格式化字符串中表示 `$` 可以使用 `$$` 进行转义。

```lumos
`The price is $$100.`
```

非法的序列中 `$` 会被保留。

```lumos
`$ $ $`
```

## 格式修饰符

格式修饰符以 `@` 开头，后接修饰符名称，修饰符名称不区分大小写。

## 原始字符串

原始字符串以 `'''` 开头和结尾，开头的 `'''` 后必须换行，结尾的 `'''` 前必须换行。<br>
<span style="color:green">原始字符串中不能使用转义序列。</span>

```lumos
'''
line1
line2
line3
'''    // 结尾的行前不能有空格
```

被解析为：

```plain
line1
line2
line3
```

如果不换行会导致报错：

```lumos
'''line1
line2
line3'''
```

原始字符串后也可以跟数量不等的括号用于自定义开头和结尾，但必须成对出现。

```lumos
'''([{<
正文
>}])'''
```

## 多行字符串

多行字符串和单行字符串类似，只是使用 `"""` 或 ` ``` ` 包裹字符串。

新行以 `|` 开头，续行以 `+` 开头，添加空格并续行以 `*` 开头。

```lumos
""" | line1
    | line2
    | line3
"""
```

使用 `+` 时需要多打一个空格来使单词之间分割，但使用 `*` 时不需要。

```lumos
""" | This is the
    +  first line.
    | This is the second line.
"""
```

```lumos
""" | This is the
    * first line.
    | This is the second line.
"""
```

以上两个均被解析为

```plain
This is the first line.
This is the second line.
```

也可以正常使用格式化表达式。

````lumos
``` | $a + $b = ${a + b}
    | $c + $d = ${c + d}
```
````

***字符串必须跨行***