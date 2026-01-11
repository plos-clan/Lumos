# 异常处理 {#title}

演示允许抛出异常与 `or` 的处理方式。示例函数通过 `@exception(allow)` 开启异常抛出，调用方使用 `or` 捕获并给出替代控制流。
> 相关语法： [允许抛出异常](../grammar/exception.md#allow-throw)，[处理异常](../grammar/exception.md#handle-exceptions)。

说明：
`or` 块可以直接 `return`、`break` 等控制语句，从而在异常时快速退出。示例中在失败分支打印提示并返回错误码，避免异常继续传播。

```lumos: exception_handling.lm```
