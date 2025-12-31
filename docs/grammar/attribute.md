
# 属性

Lumos 属性是用于修饰变量、函数和类的特殊语法，用于指定它们的特定行为或属性。  
Lumos 的属性以 `@` 符号开头，后面跟着属性的名称和可选的参数。  
`@属性名(参数列表)`  

属性名与变量名等名称不同，可以包含 `- + ! ?` 等符号。

`@` 符号后可以空格。

可以为常用属性设置新的名字，如

```lumos
using @属性 = @属性1 @属性2 @属性3;
using @属性(xxx) = @属性1 @属性2(xxx) @属性3;
```

可以重设默认属性（`@default`），如：

```lumos
// 设置所有对象的通用属性
using @default = @属性1 @属性2 @属性3;
// 设置每种对象的属性
using @default fun = @属性1 @属性2 @属性3;
using @default var = @属性1 @属性2 @属性3;
// 重设默认属性
using @default = @reset;
```

不能重设 `@reset` 属性。

## 通用属性

### 可见性

类与命名空间可见性

- `@public`: 声明变量或函数或类为公共的，可以被外部访问
- `@private`: 声明变量或函数为私有的，只能被当前类访问
- `@protected`: 声明变量或函数为受保护的，可以被当前类和子类访问

模块与编译单元可见性

- `@hidden`: 类似 C 语言中的 static，将函数的可见性限制在当前编译单元内。
- `@export`: 导出变量或函数或类，使其可以被其它模块访问。

<!-- 当我们声明一个函数时，其默认可见性为 `public`，而变量的默认可见性为 `hidden`。 -->

对于可见性，我们可以应用简写：

```lumos
var+ my_var1; // public
var- my_var2; // private
var? my_var3; // protected
```

### 断言

断言属性和普通断言的区别是，断言属性会在每次读写时检查，而普通断言只在断言处检查一次。

相同点是它们都只在调试版本检查。

```lumos
@assert(arg >= 0)
act[io.out] my_func(var i32 arg) -> unit { // 在函数开始处检查
    println(arg);                 // 检查，值未改变
    arg = -1;                     // 再次检查，打印错误信息并终止
}
```

```lumos
act[io.out] my_func(i32 arg) -> unit {
    assert(arg >= 0); // 只在此处检查
    println(arg);     // 不会再次检查
    // arg = -1;      // 错误：arg 是不可变的
}
```

### 可用性

- `@deprecated("消息")`: 标记变量、函数或类为已废弃的，提供一个可选的提示消息说明替代方案或原因。
- `@deleted("消息")`: 标记变量、函数或类为已删除的，提供一个可选的提示消息说明替代方案或原因。
- `@replaced-by(函数或变量名)`: 标记某个函数已被其它函数替代，同时标记其已被废弃。

## 函数属性

- `@generator`: 声明函数是一个生成器，可以多次返回直到 `return` 后为空。

  它会将函数改变成返回一个仅可迭代对象

  ```lumos
  @generator
  fun my_func(i32 n) -> i32 {
    for (var i32 i = 0; i < n; i++) {
      return i;
    }
    return; // 空的 return，函数终止
  }
  ```

  for (val i : my_func(10)) {
    println(i);
  }

  ```

- `@mt-lock`：

  对函数的调用加锁使其同一时间只能被一个线程调用。  
  对类加锁使其同一时间只能有一个成员函数被调用。

  ```lumos
  var i32 count = 0;

  @mt-lock
  act call_count() -> unit {
  }
  ```

- `@cache`：允许运行时缓存函数返回值，以减少重复计算。  

  <span style="color:purple">`cache` 属性隐式地包含 `pure` 属性</span>  
  <span style="color:green">具体是否缓存取决于编译器实现</span>

  ```lumos
  @cache
  def fib(i32 n) -> i32 {
    if (n <= 1) {
      return n;
    }
    return fib(n - 1) + fib(n - 2);
  }
  ```

### 调用约定

- `@cdecl`: 调用约定
- `@stdcall`: 调用约定
- `@fastcall`: 调用约定
- `@thiscall`: 调用约定

  使用 thiscall 时，第一个参数无需参数名，会自动被编译器识别为 this 指针

  ```lumos
  @thiscall
  act[obj.write] my_func(MyClass, int value) {
  }
  ```

- `@no-stack-frame`: 无栈帧

  编译器不会为函数创建栈帧，这样可以减少函数调用的开销。  
  如果函数中变量过多，寄存器不足，会导致编译错误。

### 多态

- `@vitural`: 声明函数为虚函数，用于实现多态。  
  也可以标记类为虚类，其中的虚函数均不可被定义而应该被其派生类定义。
- `@override`: 声明函数为重写函数，用于实现多态。  
  这要求基类中必须有对应的虚函数。
- `@final`: 声明函数为最终函数，禁止重写。

---

- `@const`: 声明函数为常函数，表明函数不依赖也不会修改任何外部状态。  
  *<span style="color:orange">warning</span>* 当一个函数可以为常函数但程序员未在声明中显式写明时。
- `@pure`: 声明函数为纯函数，表明函数不依赖任何外部状态（除了传入指针指向的内存）也不会修改任何外部状态。  
  *<span style="color:orange">warning</span>* 当一个函数可以为纯函数但程序员未在声明中显式写明时。
  > 注意：常量表达式等编译期可以确定的不算外部状态
- `@noreturn`: 声明函数不会返回（等同于在函数签名后使用 `-` 标记）。
- `@rettwice`: 声明函数会返回两次。

---

- `@nonull(参数名列表)`: 声明函数的参数不会为 null。参数名列表是一个逗号分隔的参数名称列表。
- `@nonull`: 声明函数的所有指针参数不会为 null。
- `@nullable`: 在使用 `@nonull` 的情况下声明函数的某个参数可能为 null。
- `@checkparam`：在函数执行前对参数进行检查而不是相信程序员。（release 版本有效）

---

- `@constexpr`: 声明函数为编译时常量表达式，可以在编译时求值。
- `@inline`: 强制内联函数，使函数在调用处展开，而不是 C 那样的建议内联。  
  inline 的函数不能被导出。  
  <span style="color:purple">无特殊需求不应该使用</span>
- `@threadlocal`: 声明变量为线程局部变量，每个线程都会有一份独立的副本。
- `@restrict(参数名或序号列表)`: 声明函数的参数是限定指针，指针之间没有重叠。

  ```lumos
  @restrict(1, 2)
  act[sys.mem] memcpy([unit] dst, [unit] src, usize n) {
  }
  ```

## 变量属性

- `@align(对齐数)`: 声明变量按照指定的对齐数对齐。  
  对齐数必须为 2 的幂，0 表示使用默认对齐数，1 表示不对齐。
- `@static`: 修饰函数中的变量，效果同 C 语言 `static` 关键字。
- `@thread`: 变量为每个线程独有，在函数中使用时同时自动设置变量为 `@static`
- `@register`: 强制变量存储在寄存器中，而不是内存中。  
  <span style="color:purple">无特殊需求不应该使用</span>
- `@volatile`：声明变量会被其它线程或硬件改变，编译器不会对其访问（读写）进行优化。

## 并行

- `@parallel`  
  循环可以并行执行，编译器会自动将其拆分为多个任务并分配给多个线程执行。

  对于循环的并行，循环体中不能修改外部变量，否则会导致数据竞争。

  ```lumos
  for (var i in .[1, 2, 3, 4, 5]) {
    // 并行执行
    // 不能修改外部状态
    barrier then {
      // 可以修改外部状态
    }
  }
  ```

  函数可以并行执行，编译器会自动将其拆分为多个任务并分配给多个线程执行。

### 示例

```lumos
@const // 输入相同得到相同输出
def square(int x) -> int {
  return x * x;
}

@inline // 强制内联
def add(int a, int b) = a + b;

@hidden
def helper(int x) -> int {
  实现
}
```
