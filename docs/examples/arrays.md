# 数组与切片 {#title}

展示 `$[...]` 数组初始化、切片与遍历。示例先创建定长数组，再通过区间索引生成切片，最后用遍历语法逐元素读取。
> 相关语法： [数组](../grammar/array.md#title)，[切片类型](../grammar/type.md#slice)。

说明：
示例使用 `$[...]` 创建数组，`arr[2..5]` 生成切片，切片具备 `.len` 属性可读长度，并用 `for (val v : s)` 进行只读遍历。

```lumos: arrays_slices.lm```
