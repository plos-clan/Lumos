
# 计量单位

**暂时未确定**

```lumos
enum Distance as unit {
    meter,
    kilometer,
    centimeter,
    millimeter,
    inch,
    // ...
}
enum unit by unit {
    meter,
    kilometer,
    centimeter,
    millimeter,
    gram,
    kilogram,
    milligram,
    // ...
}
```

我们可以按照如下方式进行类型转换：

```lumos
val one_kilometer = 1.0 as kilometer;
f64 distance = one_kilometer in meter;
```

```lumos
Unit
    | "cm" | "mm" | "m" | "km"
    | "g" | "mg" | "kg"
    | "s" | "ms" | "us" | "ns"
    | "Hz" | "kHz" | "MHz" | "GHz"
    | "N" | "Pa" | "J" | "W" | "C" | "V" | "F" | "Ω" | "S" | "H" | "T" | "Wb" | "lm" | "lx" | "Bq" | "Gy" | "Sv" | "kat"
    | "mol" | "cd" | "rad" | "sr"
    | "L" | "mL" | "m³" | "L/s" | "mL/s"
    | "°C" | "K"
    | "°" | "′" | "″"
    | "m/s" | "m/s²" | "m²" | "m³" | "m/s³" | "m/s⁴" | "m/s⁵" | "m/s⁶" | "m/s⁷" | "m/s⁸" | "m/s⁹" | "m/s¹⁰"
    | "m²/s" | "m²/s²" | "m²/s³" | "m²/s⁴" | "m²/s⁵" | "m²/s⁶" | "m²/s⁷" | "m²/s⁸" | "m²/s⁹" | "m²/s¹⁰"
    | "m³/s" | "m³/s²" | "m³/s³" | "m³/s⁴" | "m³/s⁵" | "m³/s⁶" | "m³/s⁷" | "m³/s⁸" | "m³/s⁹" | "m³/s¹⁰"
    | "m/s²" | "m/s³" | "m/s⁴" | "m/s⁵" | "m/s⁶" | "m/s⁷" | "m/s⁸" | "m/s⁹"
```
