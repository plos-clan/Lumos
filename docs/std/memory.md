
# 内存

### memset

将 n 个字节设置为 val。

```lumos
@returns(dst)
@write(dst, n)
fn memset(void* dst, u8 val, usize n) -> void*;
```

同时具有 8 16 32 64 位版本，属性同上，声明如下。

```lumos
fn memset8(bits8* dst, u8 val, usize n) -> void*;
fn memset16(u16* dst, u16 val, usize n) -> void*;
fn memset32(u32* dst, u32 val, usize n) -> void*;
fn memset64(u64* dst, u64 val, usize n) -> void*;
```

### memcpy

将 n 个字节从 src 复制到 dst。

```lumos
@returns(dst)
@nonull(src, dst)
@read(src, n)
@write(dst, n)
fn memcpy(void* dst, void val* src, usize n) -> void*;
```
