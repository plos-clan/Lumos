
# 注释

Lumos 支持单行注释和多行注释，使用方式与 C 语言类似

## 单行注释

单行注释以 `//` 开头，直到行尾结束

> token 中的 text 为注释内容，`//` 及之后的空白不会被包含在 text 中
>
> - `//aaa` 与 `// aaa` 的 text 都为 `aaa`
>
> > 空白包括空格、制表符等

## 多行注释

多行注释以 `/*` 开头，以 `*/` 结尾

> token 中的 text 为注释内容，`/*` 及之后的空白、`*/` 及之前的空白不会被包含在 text 中
>
> > 空白包括空格、制表符、回车等

<span style="color:green">不能嵌套的，别想了</span>
