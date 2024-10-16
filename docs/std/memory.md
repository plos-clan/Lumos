
# 内存

### memcpy

```lumos
@returns(dst)
@nonull(src, dst)
@read(src, size)
@write(dst, size)
fn memcpy(void* dst, void val* src, usize size) -> void*;
```
