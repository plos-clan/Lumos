
# 随机数 `std.random`

## 类

```lumos
class RNG {
  @public:
    act RNG(i32 seed);
    act RNG(i64 seed);

    fun as int;
    fun as float;
    fun as bool;
}
```

## 函数（使用梅森旋转算法）

```lumos
act srand(i32 seed);
act srand(i64 seed);
act rand();
fun randf() -> float; // 浮点随机数 [0,1] 均匀分布
fun randn() -> float; // 浮点随机数 正态分布
\fun> rand[8,16,32,64]();
\fun> randf[32,64]();
\fun> randn[32,64]();
fun randb();           // 布尔随机数
fun randbits(usize n); // n 位随机数
```

```lumos
fun randf() -> float {
    return float \fun> rand / INT_MAX;
}

fun randn() -> float {
    return sqrt(-2 * \fn> log \fn> randf) * cos(2 * PI * \fn> randf);
}
```
