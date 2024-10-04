
# 标准库

**关于异常**：标准库的每个函数都有开启异常和关闭异常的版本。因为 Lumos 默认是关闭异常的。<br>
<span style="color:green">为了在全局开启异常，你可以编辑项目根目录的 [`lumos-config.lm`](../macro/config.md)。</span>

## 容器 `std.container`

容器通用成员定义（如果 `xxx` 是一个容器）：<br>
<span style="color:green">通用成员并不是在每种容器上都可用，而是规定容器中同名成员存在时的行为。</span>

*只读变量 或 `getter`*

```lumos
xxx.size;          // 表示容器内元素所占据的字节数
xxx.length;        // 表示容器内元素的个数
xxx.capacity;      // 表示容器内元素的最大个数
xxx.is_resizeable; // 表示容器是否可以调整大小
xxx.is_empty;      // 表示容器是否为空
```

*函数*

```lumos
xxx.clear();       // 清空容器
xxx.resize(n);     // 调整容器大小
xxx.reserve(n);    // 保留容器大小
xxx.shrink();      // 缩小容器大小为实际大小
xxx.push(x);       // 向容器的默认插入位置添加元素
xxx.pop();         // 从容器的默认删除位置删除元素
xxx.append(x);     // 向容器的末尾添加元素
xxx.prepend(x);    // 向容器的开头添加元素
xxx.insert(n, x);  // 向容器的第 n 个位置添加元素
xxx.erase(n);      // 从容器的第 n 个位置删除元素
xxx.at(n);         // 获取容器的第 n 个元素
xxx.get(k);        // 获取容器的键为 k 的元素
xxx.set(k, v);     // 设置容器的键为 k 的元素为 v
xxx.remove(k);     // 删除容器的键为 k 的元素
```

## 标准输入输出

```lumos
print("Hello world!");   // 输出不换行
println("Hello world!"); // 输出并换行

int a;
print("Please input a number: ");
scan(a); // 输入一个整数
```

## 字符串操作

## 文件操作 `std.file`

### 函数

```lumos
open("file.txt", read); // 以只读方式打开文件
```
