
# 类型

## 基本类型

| 简写 | 全称   | 解释      |
|------|--------|-----------|
|      | bool   | 布尔值    |
|      | byte   | 字节      |
|      | char8  | 8 位字符  |
|      | char16 | 16 位字符 |
|      | char32 | 32 位字符 |

*二进制类型*

`bit` 类型不允许算术运算，只允许位运算。<br>
`flag` 用于表示标志位，其在 `bit` 类型的基础上禁用了移位运算。

| 简写 | 全称   | 解释            |
|------|--------|-----------------|
|      | bit8   | 8 位二进制数据  |
|      | bit16  | 16 位二进制数据 |
|      | bit32  | 32 位二进制数据 |
|      | bit64  | 64 位二进制数据 |
|      | flag8  | 8 位标志位      |
|      | flag16 | 16 位标志位     |
|      | flag32 | 32 位标志位     |
|      | flag64 | 64 位标志位     |

*数字类型*

整数使用 `i` 或 `u` 前缀表示有符号或无符号，浮点数使用 `f` 前缀表示浮点数。

| 简写 | 全称    | 解释            |
|------|---------|-----------------|
| i8   | int8    | 8 位整数        |
| i16  | int16   | 16 位整数       |
| i32  | int32   | 32 位整数       |
| i64  | int64   | 64 位整数       |
| u8   | uint8   | 8 位无符号整数  |
| u16  | uint16  | 16 位无符号整数 |
| u32  | uint32  | 32 位无符号整数 |
| u64  | uint64  | 64 位无符号整数 |
| f32  | float32 | 32 位浮点数     |
| f64  | float64 | 64 位浮点数     |

!!! Note
    非字长扩展的基本数据类型变量的读取和写入都应是原子的。

    ```lumos
    f64 my_var = 1.1; // 写入是原子的
    println(my_var);  // 读取是原子的
    my_var += 1;      // 自增不是原子的，因为它是读取后运算再写入
    ```

### 类型通配

- `inttype` 整数类型
  - 实现整数所具有的运算符
  - 只能存整数而不能存实数
- `floattype` 浮点数类型 (实现浮点数所具有的运算符的类型)
- `numtype` 通用数字类型 (实现数字所具有的运算符的类型)
  - `+ - * / %`

### 字长扩展

*数字类型*

| 简写 | 全称     | 解释             |
|------|----------|------------------|
| i128 | int128   | 128 位整数       |
| u128 | uint128  | 128 位无符号整数 |
| f16  | float16  | 16 位浮点数      |
| f128 | float128 | 128 位浮点数     |

## 扩展类型

| 简写 | 全称            | 解释               |
|------|-----------------|--------------------|
| c32  | complex float32 | 32 位浮点复数      |
| c64  | complex float64 | 64 位浮点复数      |
| vec2 | vector2 float32 | 2 维 32 位浮点向量 |
|      | vector2     xxx | 可接所有数字类型   |
| vec3 | vector3 float32 | 3 维 32 位浮点向量 |
|      | vector3     xxx | 可接所有数字类型   |
| vec4 | vector4 float32 | 4 维 32 位浮点向量 |
|      | vector4 xxx     | 可接所有数字类型   |

## 扩展类型

| 简写 | 全称                                | 解释     |
|------|-------------------------------------|----------|
| mpz  | multiple precision integers         | 大整数   |
| mpq  | multiple precision rational numbers | 大有理数 |
| mpf  | multiple precision floating-point   | 大浮点数 |

## 指针类型

```lumos
int a = 1;
int* b = &a;
```

## 引用类型

```lumos
int a = 1;
ref b = a;
```

## 多个返回类型

```lumos
variant IntOrVoid = int | void;

fn foo(int arg) -> IntOrVoid {
    if arg < 0 {
        return void;
    }
    return arg;
}

fn main() {
    var value = foo(10);
    switch value {
    int:  println("返回值是 int: ", value);
    void: println("返回值是 void");
    }
}
```

```lumos
variant IntOrVoid = int | void;

fn foo(int arg) -> IntOrVoid {
    if arg < 0 {
        return void;
    }
    return arg;
}

fn main() {
    ref value = foo(10);
    switch value {
    int:  println("返回值是 int: ", value);
    void: println("返回值是 void");
    }
}
```

## 类型转换

```lumos
int a = 1;
float b = a;
```

## 类型别名

```lumos
using MyInt = int;
```

## 比较类型

使用 `is` `!is` 运算符来比较类型是否相同。<br>
用法为 `变量 is 类型` `类型 is 类型`，此处的类型可以为类型别名。

```lumos
val a = 1;
if (a is int) {
    println("a is int.");
} else {
    println("a is not int.");
}
```

## 类型属性

- `@limit(最小值, 最大值)` 限制数值的范围。
