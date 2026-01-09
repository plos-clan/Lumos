# 容器 `std.container`

## 通用成员

容器通用成员定义（如果 `xxx` 是一个容器）：  
通用成员并不是在每种容器上都可用，而是规定容器中同名成员存在时的行为。

### 只读属性

```lumos
\val> size;          // 表示容器内元素所占据的字节数
\val> length;        // 表示容器内元素的个数
\val> capacity;      // 表示容器内元素的最大个数
\val> is_resizeable; // 表示容器是否可以调整大小
\val> is_empty;      // 表示容器是否为空
```

### 通用函数

```lumos
// 清空容器
fun clear() -> unit;

// 调整容器大小
fun resize(usize n) -> unit;

// 保留容器容量
fun reserve(usize n) -> unit;

// 缩小容器容量到实际大小
fun shrink_to_fit() -> unit;

// 向容器默认位置添加元素
fun push(T x) -> unit;

// 从容器默认位置删除元素
fun pop() -> T;

// 向容器末尾添加元素
fun append(T x) -> unit;

// 向容器开头添加元素
fun prepend(T x) -> unit;

// 在指定位置插入元素
fun insert(usize pos, T x) -> unit;

// 删除指定位置的元素
fun erase(usize pos) -> T;

// 获取指定位置的元素
fun at(usize pos) -> T;

// 获取指定键的元素值（映射专用）
fun get(K key) -> Option<V>;

// 设置指定键的元素值（映射专用）
fun set(K key, V value) -> unit;

// 删除指定键的元素（映射专用）
fun remove(K key) -> Option<V>;

// 判断是否包含元素
fun contains(T value) -> bool;

// 获取元素在容器中的位置
fun find(T value) -> Option<usize>;

// 遍历容器
fun for_each(act[sys.fn] f) -> unit;

// 克隆容器
fun clone() -> T;
```

### 运算符重载

```lumos
// 连接：创建新容器包含两个容器的所有元素
c1 + c2

// 差集：创建新容器包含在c1但不在c2的元素
c1 - c2

// 交集：创建新容器包含在两个容器中都存在的元素
c1 & c2

// 并集：同 c1 + c2
c1 | c2

// 对称差：创建新容器包含在两个容器中但不同时存在的元素
c1 ^ c2

// 左移：向容器默认插入位置添加元素
c << x

// 右移：从容器默认删除位置删除元素
c >> x

// 索引访问
c[i]
```

### 类型转换

```lumos
// 容器转布尔：如果容器为空则为 false，否则为 true
fun \type> to_bool() -> bool;

// 容器转字符串
fun \type> to_string() -> str;
```

## 迭代器

所有容器都提供迭代器支持。

```lumos
\type> Iterator<typename T> {
    // 检查是否有下一个元素
    fun has_next() -> bool;

    // 获取下一个元素
    fun next() -> T;

    // 获取当前位置
    fun position() -> usize;
}

// 获取容器迭代器
fun iter() -> Iterator<T>;

// 获取可变迭代器
fun iter_mut() -> Iterator<T>;

// 获取反向迭代器
fun iter_rev() -> Iterator<T>;
```

### 迭代器使用示例

```lumos
use std.container.*;

act main() {
    val numbers = [1, 2, 3, 4, 5];

    // 使用 for 循环
    for num in numbers {
        println(num);
    }

    // 手动迭代
    var it = numbers.iter();
    while it.has_next() {
        println(it.next());
    }
}
```

## 有序集合

按照插入顺序排列的集合类型。

### 数组 Array

```lumos
\type> Array<typename T, usize N> {
    fun new() -> \type> Array<T, N>;
    fun from([T] slice) -> \type> Array<T, N>;
}

// 固定大小数组
[i32; 5] arr = [1, 2, 3, 4, 5];
arr[0] = 10;
```

**特性**：

- 固定大小，栈分配
- 零开销抽象
- 支持编译期大小检查

### 列表 List

```lumos
\type> List<typename T> {
    fun new() -> \type> List<T>;
    fun with_capacity(usize cap) -> \type> List<T>;
}

// 动态数组
val list = List::<i32>::new();
list.push(1);
list.push(2);
list.push(3);
```

**特性**：

- 动态大小，堆分配
- O(1) 随机访问
- O(1) 末尾插入/删除
- O(n) 首部插入/删除

### 链表 LinkedList

```lumos
\type> LinkedList<typename T> {
    fun new() -> \type> LinkedList<T>;
}

val list = LinkedList::<i32>::new();
list.push_back(1);
list.push_front(0);
```

**特性**：

- O(1) 两端插入/删除
- O(n) 随机访问
- O(n) 空间开销

### 队列 Queue

```lumos
\type> Queue<typename T> {
    fun new() -> \type> Queue<T>;

    // 入队
    fun push(T x) -> unit;

    // 出队
    fun pop() -> T;

    // 查看队头
    fun front() -> T;
}

val q = Queue::<i32>::new();
q.push(1);
q.push(2);
i32 x = q.pop(); // x = 1
```

**特性**：

- FIFO（先进先出）
- O(1) 入队/出队

### 栈 Stack

```lumos
\type> Stack<typename T> {
    fun new() -> \type> Stack<T>;

    // 压栈
    fun push(T x) -> unit;

    // 弹栈
    fun pop() -> T;

    // 查看栈顶
    fun top() -> T;
}

val stack = Stack::<i32>::new();
stack.push(1);
stack.push(2);
i32 x = stack.pop(); // x = 2
```

**特性**：

- LIFO（后进先出）
- O(1) 压栈/弹栈

## 无序集合

元素排列与插入顺序无关的集合类型。

### 哈希集 HashSet

```lumos
\type> HashSet<typename T> {
    fun new() -> \type> HashSet<T>;
    fun with_capacity(usize cap) -> \type> HashSet<T>;
    fun insert(T x) -> bool;         // 返回是否插入成功
    fun remove(T x) -> bool;         // 返回是否移除成功
    fun contains(T x) -> bool;
}

val set = HashSet::<i32>::new();
set.insert(1);
set.insert(2);
bool found = set.contains(1);
```

**特性**：

- O(1) 平均查找/插入/删除
- 无序存储
- 需要实现 Hash trait

## 排序集合

元素自动排序的集合类型。

### 集合 Set

```lumos
\type> Set<typename T> {
    fun new() -> \type> Set<T>;
    fun insert(T x) -> bool;
    fun remove(T x) -> bool;
    fun contains(T x) -> bool;
    fun min() -> T;
    fun max() -> T;
}

val set = Set::<i32>::new();
set.insert(3);
set.insert(1);
set.insert(2);  // 自动排序：1, 2, 3
```

**特性**：

- 红黑树实现
- O(log n) 插入/删除/查找
- 自动排序

### 优先队列 PriorityQueue

```lumos
\type> MinPQ<typename T> {  // 最小堆
    fun new() -> \type> MinPQ<T>;
    fun push(T x) -> unit;
    fun pop() -> T;
    fun top() -> T;
}

\type> MaxPQ<typename T> {  // 最大堆
    fun new() -> \type> MaxPQ<T>;
    fun push(T x) -> unit;
    fun pop() -> T;
    fun top() -> T;
}

val pq = MinPQ::<i32>::new();
pq.push(3);
pq.push(1);
pq.push(2);
i32 min = pq.pop();  // min = 1
```

**特性**：

- 堆实现
- O(log n) 插入/删除
- O(1) 查看最值

## 有序映射

按插入顺序维护键值对的映射类型。

### 数组映射 ArrayMap

```lumos
\type> ArrayMap<typename K, typename V, usize N> {
    fun new() -> \type> ArrayMap<K, V, N>;
    fun set(K key, V value) -> unit;
    fun get(K key) -> Option<V>;
}
```

**特性**：

- 固定大小，栈分配
- O(n) 查找

### 列表映射 ListMap

```lumos
\type> ListMap<typename K, typename V> {
    fun new() -> \type> ListMap<K, V>;
    fun set(K key, V value) -> unit;
    fun get(K key) -> Option<V>;
}
```

**特性**：

- 动态大小，基于列表
- O(n) 查找

### 链表映射 LinkedListMap

```lumos
\type> LinkedListMap<typename K, typename V> {
    fun new() -> \type> LinkedListMap<K, V>;
    fun set(K key, V value) -> unit;
    fun get(K key) -> Option<V>;
}
```

**特性**：

- 基于链表
- O(n) 查找

## 无序映射

哈希表实现的映射类型。

### 哈希映射 HashMap

```lumos
\type> HashMap<typename K, typename V> {
    fun new() -> \type> HashMap<K, V>;
    fun with_capacity(usize cap) -> \type> HashMap<K, V>;
    fun set(K key, V value) -> unit;
    fun get(K key) -> Option<V>;
    fun remove(K key) -> Option<V>;
    fun contains_key(K key) -> bool;
    fun keys() -> Iterator<K>;
    fun values() -> Iterator<V>;
    fun entries() -> Iterator<(K, V)>;
}

val map = HashMap::<str, i32>::new();
map.set("age", 25);
Option<i32> age = map.get("age");
```

**特性**：

- O(1) 平均查找/插入/删除
- 无序存储
- 需要实现 Hash trait

## 排序映射

红黑树实现的映射类型。

### 映射 Map

```lumos
\type> Map<typename K, typename V> {
    fun new() -> \type> Map<K, V>;
    fun set(K key, V value) -> unit;
    fun get(K key) -> Option<V>;
    fun remove(K key) -> Option<V>;
    fun contains_key(K key) -> bool;
    fun first() -> Option<(K, V)>;
    fun last() -> Option<(K, V)>;
    fun keys() -> Iterator<K>;
    fun values() -> Iterator<V>;
}

val map = Map::<str, i32>::new();
map.set("alice", 25);
map.set("bob", 30);
// 自动按键排序
```

**特性**：

- O(log n) 查找/插入/删除
- 按键自动排序
- 范围查询支持

## 容器性能对比表

| 容器 | 索引 | 前插 | 后插 | 查找 | 排序 | 内存 |
|------|------|------|------|------|------|------|
| Array | O(1) | - | - | O(n) | ✗ | 栈 |
| List | O(1) | O(n) | O(1) | O(n) | ✗ | 堆 |
| LinkedList | O(n) | O(1) | O(1) | O(n) | ✗ | 堆 |
| Queue | - | - | O(1) | O(n) | ✗ | 堆 |
| Stack | - | - | O(1) | O(n) | ✗ | 堆 |
| HashSet | - | - | - | O(1) | ✗ | 堆 |
| Set | - | - | - | O(log n) | ✓ | 堆 |
| HashMap | - | - | - | O(1) | ✗ | 堆 |
| Map | - | - | - | O(log n) | ✓ | 堆 |

## 使用示例

```lumos
use std.container.*;

act main() {
    // 列表操作
    val numbers = List::<i32>::new();
    numbers.push(10);
    numbers.push(20);
    numbers.push(30);

    for num in numbers {
        println(num);
    }

    // 哈希映射
    val map = HashMap::<str, i32>::new();
    map.set("apple", 5);
    map.set("banana", 3);
    map.set("orange", 2);

    if let Option::Some(count) = map.get("apple") {
        println(count);
    }

    // 优先队列
    val pq = MinPQ::<i32>::new();
    [10, 3, 7, 1, 5].iter().for_each(|x| pq.push(x));

    while !pq.is_empty() {
        println(pq.pop());  // 输出：1 3 5 7 10
    }
}
```

---

相关内容：类型系统见 [类型系统](../grammar/type.md)。
