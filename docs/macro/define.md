
# 宏定义

对于某个宏定义：

- `#define` 定义
- `#redef` 重写
- `#undef` 取消

  取消某个普通宏

  ```lumos
  #define XXX
  #undef XXX
  ```

  取消某个宏函数

  ```lumos
  #define XXX(name) name
  #undef XXX(1) // XXX(N) 表示有 N 个参数
  ```

  重复的 `#define` 会导致警告，而 `#redef` 不会。<br>
  `#redef` 前没有 `#define` 会导致警告。<br>
  请合理使用宏定义。

  ```lumos
  #define XXX 1
  #redef XXX  2
  ```

> 编译器内置宏定义以 `#%` 开头

```lumos
#define linux #%config.os.linux
```
