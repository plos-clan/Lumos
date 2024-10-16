
# 类汇编

使用 `@asm` 属性在函数中启用类汇编模式。

```lumos
@asm
fn my_func {
    return 0;
}
```

```asm
my_func:
    mov rax, 0
    ret
```

## 返回值

类汇编函数的返回值只能是能够通过通用寄存器 `rax`/`eax` 传递的类型。<br>
例如：`char` `int` `float` `void*` `usize` 等。
