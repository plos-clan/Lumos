
# 随机数 `std.random`

## 类

```lumos
class RNG {
  @public:
    RNG(i32 seed);
    RNG(i64 seed);

    fn as int;
    fn as float;
    fn as bool;
}
```

## 函数（使用梅森旋转算法）

```lumos
srand(i32 seed);
srand(i64 seed);
rand();
randf(); // 浮点随机数 [0,1] 均匀分布
randn(); // 浮点随机数 正态分布
\fn> rand\[8,16,32,64]();
\fn> randf\[32,64]();
\fn> randn\[32,64]();
randb();           // 布尔随机数
randbits(usize n); // n 位随机数
```

```lumos
fn randf() -> float {
    return float \fn> rand / INT_MAX;
}

fn randn() -> float {
    return sqrt(-2 * \fn> log \fn> randf) * cos(2 * PI * \fn> randf);
}
```
