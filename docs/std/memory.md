
# 内存

### memset

将 n 个字节设置为 val。

```lumos
@returns(dst)
@write(dst, n)
act[sys.mem] memset([unit] dst, u8 val, usize n) -> [unit];
```

同时具有 8 16 32 64 位版本，属性同上，声明如下。

```lumos
act[sys.mem] memset8([bits8] dst, u8 val, usize n) -> [unit];
act[sys.mem] memset16([u16] dst, u16 val, usize n) -> [unit];
act[sys.mem] memset32([u32] dst, u32 val, usize n) -> [unit];
act[sys.mem] memset64([u64] dst, u64 val, usize n) -> [unit];
```

### memcpy

将 n 个字节从 src 复制到 dst。

```lumos
@returns(dst)
@nonull(src, dst)
@read(src, n)
@write(dst, n)
act[sys.mem] memcpy([unit] dst, [unit] src, usize n) -> [unit];
```
