
# 文件 {#title}

## 导入文件 {#file-include}

- `#include` 包含另一个文件的内容
- `#import` 导入另一个文件的语法树

`#include` 的尖括号/引号用法与 C 类似（用于区分搜索路径与当前文件）。  
推荐优先使用 `use` 导入模块；`#include`/`#import` 仅用于特殊场景。

## 控制导入 {#include-control}

```lumos
#once
```

与 C 中的 `#pragma once` 类似，让文件只被包含一次。

---

相关内容：文件查找规则见 [文件查找规则](filesearch.md)。
