# 模块与命名空间 {#title}

演示 `use` 的常见导入方式，以及根命名空间与显式命名空间的写法。示例覆盖默认命名空间、显式命名空间、根命名空间三种导入策略，并展示按符号导入以减小命名污染。
> 相关语法： [模块](../grammar/modules.md#import)，[关键字参考](../lexical/keyword.md#title)。

说明：
`use "std"` 使用默认命名空间；`use "std" -> std` 明确绑定到 `std` 命名空间，避免与本地符号冲突；`use "std" -> ::` 导入到根命名空间，适合小型脚本但容易污染全局；`use std.println` 只导入单个符号，适合精细控制可见性。

```lumos: modules_use.lm```
