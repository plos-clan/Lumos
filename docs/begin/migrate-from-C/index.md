
# 从 C 语言迁移

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
using "std";

act[io.out] main() {
  std.println("Hello world!");
  return 0;
}
```

:::endcolumn
