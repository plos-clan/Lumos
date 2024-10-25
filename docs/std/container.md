
# 容器 `std.container`

## 通用成员

容器通用成员定义（如果 `xxx` 是一个容器）：<br>
<span style="color:green">通用成员并不是在每种容器上都可用，而是规定容器中同名成员存在时的行为。</span>

### 只读变量 或 `getter`

```lumos
\val> size;          // 表示容器内元素所占据的字节数
\val> length;        // 表示容器内元素的个数
\val> capacity;      // 表示容器内元素的最大个数
\val> is_resizeable; // 表示容器是否可以调整大小
\val> is_empty;      // 表示容器是否为空
```

### 函数

```lumos
clear();       // 清空容器
resize(n);     // 调整容器大小
reserve(n);    // 保留容器大小
shrink();      // 缩小容器大小为实际大小
push(x);       // 向容器的默认插入位置添加元素
pop();         // 从容器的默认删除位置删除元素
append(x);     // 向容器的末尾添加元素
prepend(x);    // 向容器的开头添加元素
insert(n, x);  // 向容器的第 n 个位置添加元素
erase(n);      // 从容器的第 n 个位置删除元素
at(n);         // 获取容器的第 n 个元素
get(k);        // 获取容器的键为 k 的元素
set(k, v);     // 设置容器的键为 k 的元素为 v
remove(k);     // 删除容器的键为 k 的元素
```

### 运算符

```lumos
\add 创建一个新容器拷贝两个容器中所有的元素
\sub 创建一个新容器拷贝第一个容器的元素但排除第二个容器中的元素
\and 创建一个新容器拷贝两个容器中相同的元素
\or  同 \add
\xor 创建一个新容器拷贝两个容器中的元素但排除两个容器中相同的元素
\lsh 将一个新元素添加到容器的默认插入位置
\rsh 从容器的默认删除位置删除一个元素
```

### 类型转换

```lumos
bool 如果容器为空则为 false，否则为 true
str  将容器转换为字符串
```

## 派生类型

将容器分为 集 与 映射 两种，集是一种只包含元素的容器，映射是一种包含键值对的容器。

### 有序集

按照插入顺序排列的集合。

```lumos
Array</typename T, usize length/> 数组;
List</typename T/> 列表;
LinkedList</typename T/> 链表;
Queue</typename T/> 队列;
Stack</typename T/> 栈;
```

### 无序集

元素排列与插入顺序无关的集合。

```lumos
HashSet</typename T/> 哈希表实现的集合;
```

### 排序集

并非按照插入顺序而是自动排序的集合。

```lumos
Set</typename T/> 红黑树实现的集合;
MinPQ</typename T/> 最小优先队列; // PriorityQueue
MaxPQ</typename T/> 最大优先队列; // PriorityQueue
```

### 有序映射

```lumos
ArrayMap</typename K, typename V, usize length/> 数组映射;
ListMap</typename K, typename V/> 列表映射;
LinkedListMap</typename K, typename V/> 链表映射;
```

### 无序映射

```lumos
HashMap</typename K, typename V/> 哈希映射;
```

### 排序映射

```lumos
Map</typename K, typename V/> 映射; // 使用红黑树实现的映射
```
