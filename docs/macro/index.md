
# 宏 {#title}

<span style="color:green">能少用就少用宏</span>

<span style="color:green">**目前纯瞎写（（（**</span>

- `#include` 包含另一个文件的内容
- `#import`
- `#once` 该文件最多被包含一次
- `#includeable` 该文件可以被包含

文件相关细节见 [文件操作](file.md) 与 [文件查找规则](filesearch.md)。

# 在宏中嵌入代码 {#title-detail}

``#` code `#``

```lumos
#```
code1
code2
code3
```#
```

#

- `#bof` 将此处设为文件头 (忽略文件上方部分)
- `#eof` 将此处设为文件尾并结束预处理
- `#;` 当前宏结尾并开始下一个宏 (用于单行多个宏)  
  单行宏中的 while 和 if 无需 end 标记
- `#`
- `#`
- `#`
- `#`
- `#`
- `#`

# 自定义宏

```lumos
// #macro 宏不能出现在单行多个宏中
#macro Y-my_macro #undef xxx #; define xxx #%ARGS
#Y-my_macro 1 // xxx 被定义为 1
#Y-my_macro 2 // xxx 被定义为 2
#Y-my_macro 3 // xxx 被定义为 3
```

> 编译器自定义宏以 `#X-` 开头
> 用户自定义宏以 `#Y-` 开头

```lumos
#set xxx = 3
#while xxx > 0
#if xxx == 1 #; break
#sub xxx 1
#end
```

```lumos
#calc xxx -= 1
```

```lumos
#set xxx = 3

#if xxx != 0
# eof {#title-extra}
#end
```

- `#system`：执行系统命令

  ```lumos
  #system curl -O http://example.com/xxx.lm
  #import "xxx.lm"
  ```

  > 不是哥们，这啥玩意啊？

```lumos
#define fib(n) \
    #if n == 0 #; 0 #\
    #elif n == 1 #; 1 #\
    #else #; fib(n - 1) + fib(n - 2) #\
    #end
```

宏定义与替换详见 [定义与替换](define.md)，条件与循环宏详见 [控制流宏](controlflow.md)。
