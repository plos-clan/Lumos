# 名称重整 (Name Mangling) {#name-mangling-name-mangling}

为了与 C++ 及其他语言区分，并支持 Lumos 特有的 `ovl` 功能，Lumos 采用以 `$$` 开头的名称重整规则。

## 基本原则 {#basic-principles}

所有重整后的名称均遵循以下格式：
`$$` + `[作用域路径]` + `[实体标识]` + `[长度]名称` + `[参数信息]` + `[返回值信息]`

### 实体标识 (Entity Tags) {#entity-tags-entity-tags}

表示当前重整对象的类型：

- `F` : 普通函数 (非重载)
- `O` : 重载函数 (标记了 `ovl`)
- `V` : 变量或常量
- `N` : 命名空间 (仅用于作用域路径)
- `S` : 结构体
- `C` : 类
- `E` : 枚举
- `T` : 内部类型或别名

### 基础类型缩写 (Type Abbreviations) {#class-type-abbreviations}

为了缩短符号长度，内置基础类型使用单字母表示：

| 类型   | 缩写 | 类型   | 缩写 |
|:-------|:-----|:-------|:-----|
| `i8`   | `c`  | `u8`   | `b`  |
| `i16`  | `s`  | `u16`  | `w`  |
| `i32`  | `i`  | `u32`  | `u`  |
| `i64`  | `l`  | `u64`  | `m`  |
| `f32`  | `f`  | `f64`  | `d`  |
| `bool` | `k`  | `unit` | `v`  |

## 函数重整规则 {#functions}

函数签名包含参数类型列表（不含参数名）和返回值。

1. **参数列表**：紧跟在函数名之后，按顺序排列类型缩写。
2. **返回值**：以 `R` 开头，后跟返回类型的缩写或重整名。

### `ovl` 的区分 {#ovl}

- **普通函数 (`F`)**：如果函数未标记 `ovl`，即使其参数不同，重整后的标识符也以 `F` 开头。通常编译器会阻止这种情况下的重名。
- **重载函数 (`O`)**：显式使用 `ovl` 的函数使用 `O` 标识，这允许链接器明确区分这是一个参与多态搜索的符号。

## 示例 {#examples}

### 1. 基础函数 {#1-functions}

```lumos
namespace math {
    def add(i32 a, i32 b) -> i32;
}
```

- **重整名**: `$$N4mathF3addiiRi`
- **解析**: 命名空间(`N4math`) -> 函数(`F3add`) -> 参数(`ii` 即两个 `i32`) -> 返回值(`Ri` 即 `i32`)

### 2. 重载函数 (`ovl`) {#2-functions-ovl}

```lumos
namespace core {
    ovl def print(i32 v) -> unit;
    ovl def print(f64 v) -> unit;
}
```

- **print(i32)**: `$$N4coreO5printiRv`
- **print(f64)**: `$$N4coreO5printdRv`
- **解析**: 使用 `O` 标识重载，参数分别为 `i` 和 `d`。

### 3. 复合类型 {#3-class}

```lumos
struct Point { val x: i32; }
def move(Point p) -> Point;
```

- **重整名**: `$$F4moveS5PointRS5Point`
- **解析**: 参数为结构体 `S5Point`，返回值为 `RS5Point`。

### 4. 引用与权限 (规划中) {#4}

对于带有权限的 `act` 函数，权限信息将编码在 `R` 之前。

```lumos
act[io.out] log(string msg) -> unit;
```

- **重整名预览**: `$$F3logP6io.out...` (具体格式待定)

---

相关内容：模块系统见 [模块系统](../grammar/modules.md)。
