
# 宏

<span style="color:green">能少用就少用宏</span>

- `#include` 包含另一个文件的内容
- `#import`
- `#once`

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

> 编译器内置宏定义以 `#%` 开头

```lumos
#define linux #%config.os.linux
```

# 控制语句

- `#if`
- `#elif`
- `#else`
- `#end`
- `#while`<br>
  `#while 1 <= 10`
- `#for`<br>
  `#for i from 1 to 10`
- `#.L1` 建立一个 label
- `#goto L1` 跳转到 label L1

# 在宏中嵌入代码

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
- `#;` 当前宏结尾并开始下一个宏 (用于单行多个宏)<br>
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

#

```lumos
#replace rule0 "xxx" -> "yyy"  // 将 xxx 替换为 yyy
#end rule0                     // 删除 rule0
#replace rule1 /xxx$/ -> /yyy/ // 将行结尾的 xxx 替换为 yyy
#end rule1                     // 删除 rule1
```

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
#  eof
#end
```

- `#enwarning` 开启某个警告
- `#diswarning` 关闭某个警告
- `#warning` 发出某个警告<br>
  `#warning "提示信息"`<br>
  `#warning W1145 "提示信息"` 定义警告编号，自定义的用 `WX` 开头

- `#system curl -O http://example.com/xxx.lm`<br>
  `#import "xxx.lm"`<br>
  > 不是哥们，这啥玩意啊？
