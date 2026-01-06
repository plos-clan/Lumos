# 名称重整

***仍不完善***

```lumos
fun foo(i32) -> unit;
fun bar(i32, i32) -> unit;
```

重整分为几种类型，以上面的函数为例：

1. 不重整，函数名直接作为导出名  
   `foo` `bar`
2. 函数名后添加参数数量  
   `foo$1` `bar$2`
3. 函数名后添加参数类型  
   `foo$i32` `bar$i32$i32`

## 基本规则

所有重整后的名称以 `$$` 开头，接着是一个字母标识符，表示名称的类型。类型标识符如下：

- `N` 表示命名空间（namespace）
- `S` 表示结构体（struct）
- `C` 表示类（class）
- `F` 表示函数（function）
- `E` 表示枚举（enum）
- `V` 表示变量（variable）
- `T` 表示内部类型（inner type）

在类型标识符之后，跟随名称的长度和名称本身。

### 示例

- `<namespace>foo` 重整为 `$$N3foo`
- `<namespace>foo.<function>bar` 重整为 `$$N3fooF3bar`

## 函数重整

对于函数，需要在重整后的函数名之后添加参数信息和返回值信息。

### 参数

重整参数时，首先重整参数类型，然后拼接上参数名，最后加上参数个数。

例如：

- `<namespace>foo.<class>bar` 重整为 `N3fooC3bar`
- 参数名 `arg` 拼接为 `N3fooC3bar3arg`

### 返回值

在所有参数之后，拼接返回值类型。如果函数没有返回值，则使用 `T4unit` 表示返回 `unit` 类型。

### 示例

1. **没有返回值的函数**

    ```lumos
    namespace foo {
      class bar {}
      def my_func(bar a) -> unit;
    }
    ```

    - `my_func` 重整为 `$$N3fooF7my_func1N3fooC3bar1aT4unit`

    解释：  
    - `N3foo` 表示命名空间 `foo`
    - `F7my_func` 表示函数 `my_func`（名称长度为 7）
    - `1N3fooC3bar1a` 表示一个参数，类型为 `N3fooC3bar`（命名空间 `foo` 下的类 `bar`），参数名为 `a`
    - `T4unit` 表示函数没有返回值

2. **有基本类型参数的函数**

    ```lumos
    namespace foo {
      def my_func(i32 a) -> unit;
    }
    ```

    - `my_func` 重整为 `$$N3fooF7my_func1T3i321aT4unit`

    解释：  
    - `N3foo` 表示命名空间 `foo`
    - `F7my_func` 表示函数 `my_func`（名称长度为 7）
    - `1T3i321a` 表示一个参数，类型为 `i32`，参数名为 `a`
    - `T4unit` 表示函数没有返回值

3. **有返回值的函数**

    ```lumos
    namespace foo {
      def my_func(int32 a) -> int64;
    }
    ```

    - `my_func` 重整为 `$$N3fooF7my_func1T5int321aT5int64`

    解释：  
    - `N3foo` 表示命名空间 `foo`
    - `F7my_func` 表示函数 `my_func`（名称长度为 7）
    - `1T3i321a` 表示一个参数，类型为 `i32`，参数名为 `a`
    - `T3i64` 表示返回值类型为 `i64`

## 其他示例

```lumos
namespace math {
  struct vector {}
  def add(vector a, vector b) -> vector;
}
```

- `add` 重整为 `$$N4mathF3add2N4mathS6vector1aN4mathS6vector1bN4mathS6vector`

解释：  

- `N4math` 表示命名空间 `math`
- `F3add` 表示函数 `add`（名称长度为 3）
- `2N4mathS6vector1aN4mathS6vector1b` 表示两个参数，第一个参数类型为 `N4mathS6vector`（命名空间 `math` 下的结构体 `vector`），参数名为 `a`，第二个参数类型相同，参数名为 `b`
- `N4mathS6vector` 表示返回值类型为 `math::vector`

```lumos
namespace app {
  enum color { red, green, blue }
  act set_color(color c) -> unit;
}
```

- `set_color` 重整为 `$$N3appF9set_color1N3appE5color1cT4unit`

解释：  

- `N3app` 表示命名空间 `app`
- `F9set_color` 表示函数 `set_color`（名称长度为 9）
- `1N3appE5color1c` 表示一个参数，类型为 `N3appE5color`（命名空间 `app` 下的枚举 `color`），参数名为 `c`
- `T4unit` 表示函数没有返回值
