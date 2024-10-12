
# 数组

使用数组可以存储多个值。

```lumos
int my_array[5] = {1, 2, 3, 4, 5};
```

当然数组中元素的类型也是支持自动推断的。

```lumos
var my_array[5] = {1, 2, 3, 4, 5};
// 推断为 int 因为所有参数都为 int

var my_array[5] = {1, 1.5, 2, 2.5, 3};
// 推断为 float 因为 int 可以自动转换为 float
//                而 float 不能自动转换为 int
```
