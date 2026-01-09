
# 计量单位 (Measure) {#measure-measure}

Lumos 引入了原生计量单位支持，通过编译期的量纲分析（Dimensional Analysis）确保物理计算的安全性，同时保持零运行时开销。

## 量纲与单位定义 {#dimensions-and-units}

使用 `measure` 关键字定义一个量纲，并在其内部定义具体的单位。每个量纲必须指定一个基准单位（通常不设比例），其他单位则相对于基准单位进行定义。

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
    min = 60 * s;           // 分
    h = 60 * min;           // 小时
}
```

## 变量声明与字面量 {#declaration-and-literals}

单位可以作为类型修饰符附加在数值类型之后，也可以直接作为字面量后缀使用。

```lumos
val distance: f64[m] = 100.0; // 显式类型标注
val time = 9.58[s];           // 自动推导为 f64[s]
val speed = 10.0[m/s];        // 复合单位
```

## 量纲分析 {#dimension-analysis}

编译器会自动检查量纲的一致性。只有相同量纲的数值才能进行加减和比较运算。

```lumos
val len1 = 10[m];
val len2 = 50[cm];
val total = len1 + len2;    // 自动换算为基准单位进行运算，结果为 10.5[m]

val weight = 5[kg];
// val error = len1 + weight; // 编译错误：量纲不匹配 (Length vs Mass)
```

乘除运算会产生新的复合量纲：

```lumos
val dist = 100[m];
val t = 10[s];
val v = dist / t;           // v 的类型为 f64[m/s]
```

## 单位换算 `in` {#unit-conversion-in}

使用 `in` 表达式可以将一个数值显式转换为另一个单位。

```lumos
val d = 1.5[km];
val d_m = d in m;           // 1500.0[m]

val speed_kmh = 120[km/h];
val speed_ms = speed_kmh in m/s; // 自动处理复合单位换算
```

## 复合量纲别名 {#compound-dimension-aliases}

可以为复杂的量纲组合定义别名，提高代码可读性。

```lumos
measure Velocity = Length / Time;
measure Acceleration = Velocity / Time;
measure Force = Mass * Acceleration;

val f: f64[Force] = 9.8[kg] * 1.0[m/s²];
```

## 零成本抽象 {#zero-cost-abstraction}

计量单位是纯编译期特性：

- **无内存开销**：`f64[m]` 在内存中与 `f64` 占用完全相同的空间。
- **无运行开销**：所有的量纲校验都在编译期完成。单位换算的乘除法在编译期进行常量折叠，不会产生额外的运行时指令。

---

相关内容：类型系统见 [类型系统](type.md#types)。
