
# 从 C 语言迁移 {#title}

:::column

```c
#include <stdio.h>

int main() {
  printf("Hello world!\n");
  return 0;
}
```

:::flex-0
=>
:::

```lumos
use "std";

act[io.out] main() {
  std.println("Hello world!");
  return 0;
}
```

:::endcolumn

---

相关内容：语法入口见 [语言参考](../../language.md)。
