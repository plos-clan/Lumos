# 算法库 `std.algorithm`

## 搜索算法

### 线性搜索

```lumos
// 查找元素首次出现的位置
fun find<typename T>([T] data, T value) -> Option<usize>;

// 查找最后一个匹配元素
fun find_last<typename T>([T] data, T value) -> Option<usize>;

// 使用谓词查找
fun find_if<typename T>([T] data, fun[T -> bool] predicate) -> Option<usize>;

// 统计元素出现次数
fun count<typename T>([T] data, T value) -> usize;

// 使用谓词统计
fun count_if<typename T>([T] data, fun[T -> bool] predicate) -> usize;
```

### 二分搜索

```lumos
// 二分查找（前提：数据已排序）
fun binary_search<typename T>([T] data, T value) -> Option<usize>;

// 查找下界（第一个 >= value 的位置）
fun lower_bound<typename T>([T] data, T value) -> usize;

// 查找上界（第一个 > value 的位置）
fun upper_bound<typename T>([T] data, T value) -> usize;

// 等于范围（返回所有等于 value 的位置范围）
fun equal_range<typename T>([T] data, T value) -> (usize, usize);
```

## 排序算法

### 基础排序

```lumos
// 快速排序
fun quicksort<typename T>(var [T] data) -> unit;

// 快速排序（范围）
fun quicksort_range<typename T>(var [T] data, usize start, usize end) -> unit;

// 快速排序（自定义比较）
fun quicksort_cmp<typename T>(
    var [T] data,
    fun[T, T -> i32] cmp,
) -> unit;

// 合并排序
fun mergesort<typename T>(var [T] data) -> unit;

// 堆排序
fun heapsort<typename T>(var [T] data) -> unit;

// 插入排序（小数组优化）
fun insertion_sort<typename T>(var [T] data, usize n) -> unit;

// 标准排序（自动选择最优算法）
fun sort<typename T>(var [T] data) -> unit;

// 稳定排序
fun stable_sort<typename T>(var [T] data) -> unit;
```

### 部分排序

```lumos
// 只排序前 n 个元素
fun partial_sort<typename T>(var [T] data, usize n) -> unit;

// 排序并找出 n 个最小元素
fun nth_element<typename T>(var [T] data, usize n) -> unit;
```

### 排序检查

```lumos
// 检查是否已排序
fun is_sorted<typename T>([T] data) -> bool;

// 检查是否已排序（自定义比较）
fun is_sorted_cmp<typename T>([T] data, fun[T, T -> bool] cmp) -> bool;
```

## 变换算法

### 映射与变换

```lumos
// 对每个元素应用函数
fun for_each<typename T>([T] data, act[T -> unit] f) -> unit;

// 变换元素
fun \type> map<typename T, typename U>([T] data, fun[T -> U] f) -> [U];

// 原地变换
fun transform<typename T>([T] data, fun[T -> T] f) -> unit;

// 复制并变换
fun \type> copy_if<typename T>([T] data, fun[T -> bool] predicate) -> [T];
```

### 填充与赋值

```lumos
// 用值填充容器
fun fill<typename T>(var [T] data, T value) -> unit;

// 用函数结果填充
fun generate<typename T>(var [T] data, fun[-> T] f) -> unit;

// 复制
fun \type> copy<typename T>([T] src) -> [T];

// 反向复制
fun \type> copy_backward<typename T>([T] src) -> [T];

// 移动
fun move<typename T>(var [T] src, var [T] dst, usize count) -> unit;
```

### 反转和旋转

```lumos
// 反转元素顺序
fun reverse<typename T>(var [T] data) -> unit;

// 旋转元素
fun rotate<typename T>(var [T] data, usize pivot) -> unit;

// 随机打乱
fun shuffle<typename T>(var [T] data) -> unit;

// 下一个排列
fun next_permutation<typename T>(var [T] data) -> bool;

// 前一个排列
fun prev_permutation<typename T>(var [T] data) -> bool;
```

## 比较与合并

### 比较

```lumos
// 比较两个序列
fun compare<typename T>([T] a, [T] b) -> i32;  // 返回 -1, 0, 1

// 检查相等
fun equal<typename T>([T] a, [T] b) -> bool;

// 字典序比较
fun lexicographical_compare<typename T>([T] a, [T] b) -> bool;

// 找出第一个不同的位置
fun mismatch<typename T>([T] a, [T] b) -> Option<usize>;
```

### 集合操作

```lumos
// 集合并集
fun set_union<typename T>([T] a, [T] b) -> [T];

// 集合交集
fun set_intersection<typename T>([T] a, [T] b) -> [T];

// 集合差
fun set_difference<typename T>([T] a, [T] b) -> [T];

// 对称差
fun set_symmetric_difference<typename T>([T] a, [T] b) -> [T];

// 包含关系检查
fun includes<typename T>([T] haystack, [T] needle) -> bool;
```

### 合并

```lumos
// 合并两个已排序的序列
fun merge<typename T>([T] a, [T] b) -> [T];

// 原地合并
fun merge_inplace<typename T>(var [T] data, usize mid) -> unit;

// 连接
fun concat<typename T>([T] a, [T] b) -> [T];
```

## 分区

```lumos
// 分区：将满足条件的元素移到前面
fun partition<typename T>(var [T] data, fun[T -> bool] predicate) -> usize;

// 稳定分区
fun stable_partition<typename T>(var [T] data, fun[T -> bool] predicate) -> usize;

// 检查是否已分区
fun is_partitioned<typename T>([T] data, fun[T -> bool] predicate) -> bool;

// 找到分区点
fun partition_point<typename T>([T] data, fun[T -> bool] predicate) -> usize;
```

## 数值算法

### 累积和与减少

```lumos
// 累积求和
fun accumulate<typename T>([T] data, T init) -> T;

// 前缀和
fun partial_sum<typename T>([T] data) -> [T];

// 相邻元素的差
fun adjacent_difference<typename T>([T] data) -> [T];

// 规约/折叠
fun reduce<typename T>([T] data, T init, fun[T, T -> T] op) -> T;
```

### 最大最小值

```lumos
// 查找最小值
fun min<typename T>([T] data) -> T;

// 查找最大值
fun max<typename T>([T] data) -> T;

// 查找最小最大值
fun minmax<typename T>([T] data) -> (T, T);

// 查找最小值位置
fun min_element<typename T>([T] data) -> usize;

// 查找最大值位置
fun max_element<typename T>([T] data) -> usize;

// 带自定义比较的最大值
fun max_element_cmp<typename T>([T] data, fun[T, T -> bool] cmp) -> usize;
```

### 求和与积

```lumos
// 求和
fun sum<typename T>([T] data) -> T;

// 乘积
fun product<typename T>([T] data) -> T;

// 加权和
fun weighted_sum<typename T>([T] data, [T] weights) -> T;

// 点积
fun dot_product<typename T>([T] a, [T] b) -> T;
```

## 使用示例

### 排序与搜索

```lumos
using std.algorithm.*;

act main() {
    var numbers = [5, 2, 8, 1, 9, 3];

    // 排序
    sort(numbers);
    println("Sorted: {}", numbers);  // [1, 2, 3, 5, 8, 9]

    // 二分搜索
    Option<usize> pos = binary_search(numbers, 5);
    if let Option::Some(idx) = pos {
        println("Found at index: {}", idx);
    }

    // 统计
    usize count = count(numbers, 5);
    println("Count of 5: {}", count);
}
```

### 变换和映射

```lumos
using std.algorithm.*;

act main() {
    [i32] data = [1, 2, 3, 4, 5];

    // 映射：每个元素乘 2
    [i32] doubled = map(data, |x| -> i32 { x * 2 });
    println("{}", doubled);  // [2, 4, 6, 8, 10]

    // 过滤
    [i32] evens = copy_if(data, |x| -> bool { x % 2 == 0 });
    println("{}", evens);  // [2, 4]

    // 反转
    var reversed = copy(data);
    reverse(reversed);
    println("{}", reversed);  // [5, 4, 3, 2, 1]
}
```

### 数值计算

```lumos
using std.algorithm.*;

act main() {
    [f32] values = [1.5, 2.5, 3.5, 4.5];

    // 求和
    f32 total = sum(values);
    println("Sum: {}", total);  // 12.0

    // 前缀和
    [f32] prefix = partial_sum(values);
    println("Prefix sum: {}", prefix);  // [1.5, 4.0, 7.5, 12.0]

    // 最大最小值
    f32 max_val = max(values);
    f32 min_val = min(values);
    println("Range: [{}, {}]", min_val, max_val);
}
```

### 集合操作

```lumos
using std.algorithm.*;

act main() {
    [i32] a = [1, 2, 3, 4];
    [i32] b = [3, 4, 5, 6];

    // 交集
    [i32] inter = set_intersection(a, b);
    println("Intersection: {}", inter);  // [3, 4]

    // 并集
    [i32] uni = set_union(a, b);
    println("Union: {}", uni);  // [1, 2, 3, 4, 5, 6]

    // 差
    [i32] diff = set_difference(a, b);
    println("Difference: {}", diff);  // [1, 2]
}
```

## 性能提示

| 算法 | 时间复杂度 | 空间复杂度 | 用途 |
|------|----------|----------|------|
| 快速排序 | O(n log n) avg, O(n²) worst | O(log n) | 一般排序 |
| 合并排序 | O(n log n) | O(n) | 稳定排序 |
| 堆排序 | O(n log n) | O(1) | 最坏情况保证 |
| 二分搜索 | O(log n) | O(1) | 已排序数据 |
| 线性搜索 | O(n) | O(1) | 未排序数据 |
