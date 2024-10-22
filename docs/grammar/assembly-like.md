
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

函数参数必须绑定到寄存器。

```lumos
@asm
fn syscall($rax size_t a, $rbx size_t b) {
    return a + b;
}
```

```asm
my_func:
    add rax, rbx
    ret
```

函数内只允许编写三元组命令，如：

```lumos
a = b + c;
a += b;
```

复杂表达式（会缓存中间状态的）是不行的，如：

```lumos
a = b + c + d;
```

## 入栈出栈

```lumos
\kwd> push my_var;
\kwd> pop  my_var;
```

## 返回值

类汇编函数的返回值只能是能够通过通用寄存器 `rax`/`eax` 传递的类型。<br>
例如：`char` `int` `float` `void*` `usize` 等。
