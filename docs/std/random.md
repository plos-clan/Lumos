# 随机数 `std.random` {#title}

## 基础概念 {#basics}

Lumos 提供高效的伪随机数生成（PRNG）支持，采用梅森旋转算法实现，具有良好的统计特性和性能。

## 全局随机数函数 {#global-rng}

### 初始化种子 {#init-seed}

```lumos
// 使用整数种子初始化
act[sys.rand] srand(i32 seed) -> unit;
act[sys.rand] srand(i64 seed) -> unit;
act[sys.rand] srand(u32 seed) -> unit;
act[sys.rand] srand(u64 seed) -> unit;

// 使用系统时间初始化（自动）
act[sys.rand] srand_auto() -> unit;

// 使用设备随机性初始化
act[sys.rand] srand_secure() -> unit;
```

### 整数随机数 {#int-random}

```lumos
// 生成随机整数 [0, INT_MAX]
fun rand() -> i32;

// 特定位宽的随机整数
fun rand8() -> u8;
fun rand16() -> u16;
fun rand32() -> u32;
fun rand64() -> u64;

// 范围内的随机整数 [min, max]
fun rand_range(i32 min, i32 max) -> i32;
fun rand_range_u32(u32 min, u32 max) -> u32;
fun rand_range_u64(u64 min, u64 max) -> u64;

// n 位的随机整数
fun randbits(usize n) -> u64;
```

### 浮点随机数 {#floats-random}

```lumos
// 生成 [0, 1) 范围的浮点数
fun randf() -> f32;
fun randf64() -> f64;

// 生成指定范围的浮点数 [min, max)
fun randf_range(f32 min, f32 max) -> f32;
fun randf_range64(f64 min, f64 max) -> f64;

// 生成正态分布的随机数（中心值 0，标准差 1）
fun randn() -> f32;
fun randn64() -> f64;

// 生成指定均值和标准差的正态分布
fun randn_normal(f32 mean, f32 stddev) -> f32;
fun randn_normal64(f64 mean, f64 stddev) -> f64;
```

### 布尔随机数 {#bool-random}

```lumos
// 生成随机布尔值
fun randb() -> bool;

// 生成概率为 p 的布尔值（p 范围 [0.0, 1.0]）
fun randb_prob(f32 p) -> bool;
```

## 随机数生成器类 {#random}

对于需要维持独立随机数状态的场景，可以使用 RNG 类。

### RNG 类定义 {#rng}

```lumos
\type> RNG {
    // 创建随机数生成器
    fun new() -> \type> RNG;

    // 使用种子创建
    fun with_seed(i64 seed) -> \type> RNG;

    // 设置种子
    fun seed(i64 seed) -> unit;

    // 重置生成器
    fun reset() -> unit;
}
```

### RNG 方法 {#random-rng}

```lumos
\type> RNG {
    // 生成随机整数 [0, INT_MAX]
    fun next_i32() -> i32;
    fun next_u32() -> u32;
    fun next_u64() -> u64;

    // 生成范围内的整数
    fun next_i32_range(i32 min, i32 max) -> i32;
    fun next_u32_range(u32 min, u32 max) -> u32;
    fun next_u64_range(u64 min, u64 max) -> u64;

    // 生成浮点数 [0, 1)
    fun next_f32() -> f32;
    fun next_f64() -> f64;

    // 生成浮点数 [min, max)
    fun next_f32_range(f32 min, f32 max) -> f32;
    fun next_f64_range(f64 min, f64 max) -> f64;

    // 生成正态分布
    fun next_gaussian() -> f32;
    fun next_gaussian64() -> f64;

    // 生成布尔值
    fun next_bool() -> bool;
    fun next_bool_prob(f32 p) -> bool;

    // 生成 n 位随机数
    fun next_bits(usize n) -> u64;
}
```

### RNG 类型转换 {#rng-detail}

```lumos
\type> RNG {
    // 转换为 i32
    fun as_i32() -> i32;

    // 转换为 f32
    fun as_f32() -> f32;

    // 转换为 bool
    fun as_bool() -> bool;
}
```

## 特殊分布 {#dist}

### 指数分布 {#exp-dist}

```lumos
// 参数为 lambda 的指数分布
fun exponential(f32 lambda) -> f32;
fun exponential64(f64 lambda) -> f64;

// 使用 RNG 对象
fun \type> RNG::next_exponential(f32 lambda) -> f32;
```

### 泊松分布 {#dist-dist}

```lumos
// 参数为 lambda 的泊松分布
fun poisson(f32 lambda) -> i32;

// 使用 RNG 对象
fun \type> RNG::next_poisson(f32 lambda) -> i32;
```

### 二项分布 {#binomial-dist}

```lumos
// 参数为 n, p 的二项分布
fun binomial(i32 n, f32 p) -> i32;

// 使用 RNG 对象
fun \type> RNG::next_binomial(i32 n, f32 p) -> i32;
```

### 均匀分布（向量） {#uniform-dist}

```lumos
// 在 n 维单位球内的均匀分布
fun sphere(usize n) -> [f32];

// 在 n 维球面上的均匀分布
fun sphere_surface(usize n) -> [f32];
```

## 使用示例 {#examples}

### 全局随机函数 {#examples-global-rng}

```lumos
use std.random.*;

act main() {
    // 初始化种子
    srand_auto();

    // 生成整数
    i32 num = rand();
    i32 range = rand_range(1, 100);
    println("Random number: {}", range);

    // 生成浮点数
    f32 decimal = randf();
    f32 prob = randf_range(0.0, 1.0);

    // 生成正态分布
    f32 normal = randn();
    println("Normal: {}", normal);

    // 生成布尔值
    bool coin = randb();
    if coin {
        println("Heads!");
    } else {
        println("Tails!");
    }
}
```

### 使用 RNG 对象 {#examples-rng}

```lumos
use std.random.*;

act main() {
    // 创建独立的随机数生成器
    var rng = RNG::with_seed(12345);

    // 生成一系列随机数
    for i in 0..10 {
        i32 num = rng.next_i32_range(1, 100);
        println("{}", num);
    }

    // 生成不同分布的随机数
    f32 uniform = rng.next_f32();
    f32 normal = rng.next_gaussian();
    i32 poisson = rng.next_poisson(3.0);

    println("Uniform: {}", uniform);
    println("Normal: {}", normal);
    println("Poisson: {}", poisson);
}
```

### 蒙特卡洛方法估算 π {#monte-carlo-pi}

```lumos
use std.random.*;

act main() {
    // 使用蒙特卡洛方法估算 π
    i32 samples = 1000000;
    i32 inside = 0;

    srand_auto();

    for i in 0..samples {
        f32 x = randf_range(0.0, 1.0);
        f32 y = randf_range(0.0, 1.0);
    
        if x * x + y * y <= 1.0 {
            inside = inside + 1;
        }
    }

    f32 pi_estimate = (inside as f32 / samples as f32) * 4.0;
    println("Estimated π: {}", pi_estimate);
}
```

### 加权随机选择 {#weighted-choice}

```lumos
use std.random.*;
use std.container.*;

act main() {
    // 权重数组
    [f32] weights = [1.0, 2.0, 3.0, 4.0];

    // 计算累积权重
    var cum_weights = List::<f32>::new();
    var sum = 0.0;

    for w in weights {
        sum = sum + w;
        cum_weights.push(sum);
    }

    // 随机选择
    srand_auto();
    f32 r = randf_range(0.0, sum);

    for i in 0..cum_weights.length {
        if r <= cum_weights.at(i) {
            println("Selected index: {}", i);
            break;
        }
    }
}
```

## 性能建议 {#performance-tips}

1. **单线程程序**：使用全局随机函数（自动管理状态）
2. **多线程程序**：每个线程使用独立的 RNG 对象（避免同步开销）
3. **需要可重现性**：使用固定种子初始化
4. **需要密码学安全**：使用 `srand_secure()` 或专门的密码学 RNG 库
5. **性能敏感**：避免频繁调用正态分布或特殊分布，考虑缓存结果

## 质量保证 {#quality-assurance}

Lumos 的随机数生成器采用 MT19937 算法，具有以下特性：

- **周期**：$2^{19937} - 1$，足够大用于大多数应用
- **统计特性**：通过 Diehard 测试和现代统计测试
- **性能**：约 1GB/s 的随机数生成速度
- **线程安全**：全局函数使用线程本地存储；RNG 对象需要显式同步

---

相关内容：数值类型见 [类型系统](../grammar/type.md#types)。
