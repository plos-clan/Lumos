
# 计量单位 (Measure) {#title}

Lumos 引入了原生计量单位支持，通过编译期的量纲分析（Dimensional Analysis）确保物理计算的安全性，同时保持零运行时开销。

> **注意**：由于单位转换的运算会在量纲操作下进行，可能导致浮点误差难以察觉。建议在精度要求高的场景下使用有理数类型，或在对误差不敏感的应用中使用浮点数类型。

## 量纲与单位定义 {#dimensions-units}

使用 `measure` 关键字定义一个量纲，并在其内部定义具体的单位。每个量纲必须指定一个基准单位（通常为 1:1 比例），其他单位则相对于基准单位进行定义。

```lumos
// 定义长度量纲
measure Length {
    m;                      // 基准单位：米
    km = 1000 * m;          // 千米
    cm = 0.01 * m;          // 厘米
    mm = 0.001 * m;         // 毫米
}

// 定义时间量纲
measure Time {
    s;                      // 基准单位：秒
    ms = 0.001 * s;         // 毫秒
    min = 60 * s;           // 分钟
    h = 60 * min;           // 小时
}
```

以上展示了部分内置量纲的定义。Lumos 预定义了现实世界中常用的单位，用户也可以根据需要定义自定义的量纲和单位。

```lumos
// 定义经验值量纲（游戏中可用）
measure Experience {
    exp;                // 基准单位：经验值
    level = 1000 * exp;
    rank = 100 * level;
}
```

**注意**：量纲名称必须全局唯一，单位名称也必须在所有量纲中唯一。

## 变量声明与字面量 {#declaration-literal}

单位可以作为类型修饰符附加在数值类型之后，也可以作为字面量后缀直接使用。

```lumos
val distance: f64[m] = 100.0; // 显式类型标注
val time = 9.58[s];           // 自动推导为 f64[s]
val speed = 10.0[m/s];        // 复合单位
```

## 量纲分析 {#dimension-analysis}

编译器会自动检查量纲的一致性。只有相同量纲的数值才能进行加减和比较运算，这确保了物理计算的类型安全。

```lumos
val len1 = 10[m];
val len2 = 50[cm];
val total = len1 + len2;    // 自动换算至基准单位后进行运算，结果为 10.5[m]

val weight = 5[kg];
val error = len1 + weight; // 编译错误：量纲不匹配 (Length vs Mass)
```

乘除运算会产生新的复合量纲。例如：

```lumos
val dist = 100[m];
val t = 10[s];
val v = dist / t;           // v 的类型为 f64[m/s]，表示速度
```

## 单位换算 `in` {#in}

使用 `in` 表达式可以将一个数值显式转换为另一个单位。

```lumos
val d = 1.5[km];
val d_m = d in m;           // 1500.0[m]

val speed_kmh = 120[km/h];
val speed_ms = speed_kmh in m/s; // 自动处理复合单位换算
```

## 复合量纲别名 {#compound-dimension-aliases}

可以为复杂的量纲组合定义别名，以提高代码的可读性和可维护性。

```lumos
measure Velocity = Length / Time;
measure Acceleration = Velocity / Time;
measure Force = Mass * Acceleration;

val f: f64[Force] = 9.8[kg] * 1.0[m/s²];
```

我们提供常见组合量纲的内置别名：

| 别名         | 定义                      |
|--------------|---------------------------|
| Speed        | Length / Time             |
| Acceleration | Speed / Time              |
| Force        | Mass * Acceleration       |
| Energy       | Force * Length            |
| Power        | Energy / Time             |
| Pressure     | Force / (Length * Length) |

## 零成本抽象 {#zero-cost-abstraction}

计量单位是纯编译期特性，具有以下特点：

- **无内存开销**：`f64[m]` 在内存中与 `f64` 占用完全相同的空间。
- **无运行时开销**：所有量纲校验都在编译期完成，单位换算的乘除法通过编译期常量折叠优化，不会产生额外的运行时指令。

---

相关内容：类型系统见 [类型系统](type.md#types)。
