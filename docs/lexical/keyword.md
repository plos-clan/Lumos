
# 关键字 (2025-10-06)

```lumos
fun main() {
    println("Hello, World!"); // Error: `act println` cannot be called from a `fun` function
}
```

- `def` 定义纯函数

  ```lumos
  def add(int a, int b) -> int {
      return a + b;
  }
  ```

- `fun` 定义逻辑纯函数

  ```lumos
  fun do_something() {
      val x = malloc(64);
      free(x);
  }
  ```

  有时我们要让有副作用的函数被视为逻辑纯的，此时使用 `unsafe` 关键字：

  ```lumos
  unsafe fun do_something() {
      println("Effectful operation");
  }
  ```

- `act` 定义副作用函数

  ```lumos
  act main() {
      println("Hello, World!");
  }
  ```

- `&Type` 引用
- `&raw Type` 原始引用（需要手动释放）

```lumos
// std::mem
unsafe fun alloc(usize size) -> &raw bytes;
unsafe fun free(&raw bytes ptr);
```

```
&raw from (ptr, len) // 从原始指针创建一个原始引用（切片）
```

承诺动作只被执行一次，常用于初始化：

```lumos
once act init() {
    println("This will only run once");
}
```

如果一个全局变量无法立刻初始化：

```lumos
// 手动初始化
val GLOBAL = late Type;
// 第一次访问时初始化
val GLOBAL = lazy {
    // initialization code
};
val GLOBAL = lazy Type {
    // initialization code
};
```
