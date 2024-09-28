
# 属性
  
Lumos 属性是用于修饰变量、函数和类的特殊语法，用于指定它们的特定行为或属性。<br>
Lumos 的属性以 `@` 符号开头，后面跟着属性的名称和可选的参数。<br>
`@属性名(参数列表)`<br>

属性名与变量名等名称不同，可以包含 `- + ! ?` 等符号。

可以为常用属性设置新的名字，如

```lumos
using @属性 = @属性1 @属性2 @属性3;
using @属性(xxx) = @属性1 @属性2(xxx) @属性3;
```

```lumos
// 多个属性，如
@属性1 @属性2 @属性3
// 可以写作
@属性1&属性2&属性3
```

可以重设默认属性，如

```lumos
// 这样来设置所有对象的通用属性
using @default = @属性1 @属性2 @属性3;
// 这样来设置每种对象的属性
using @default fn = @属性1 @属性2 @属性3;
using @default var = @属性1 @属性2 @属性3;
```

```lumos
#set @default = @属性1 @属性2 @属性3;
#clear @default;
```

## 通用属性

### 可见性

- `@public`: 声明变量或函数或类为公共的，可以被外部访问
- `@private`: 声明变量或函数为私有的，只能被当前类访问
- `@protected`: 声明变量或函数为受保护的，可以被当前类和子类访问
- `@hidden`: 类似 C 语言中的 static，将函数的可见性限制在当前编译单元内。

### 旧版本兼容

- `@deprecated("消息")`: 标记变量、函数或类为已废弃的，提供一个可选的提示消息说明替代方案或原因。
- `@deleted("消息")`: 标记变量、函数或类为已删除的，提供一个可选的提示消息说明替代方案或原因。
- `@replaced-by(函数名)`: 标记某个函数已被其它函数替代，同时标记其已被废弃。

## 函数属性

- `@generator`: 声明函数是一个生成器，可以多次返回直到 `return` 后为空。

  它会将函数改变成返回一个仅可迭代对象

  ```lumos
  @generator
  fn my_func(int n) -> int {
    for (int i = 0; i < n; i++) {
      return i;
    }
    return; // 空的 return，函数终止
  }

  for (int i : my_func(10)) {
    println(i);
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
  fn my_func(MyClass, int value) {
    member = value;
  }
  ```

- `@no-stack-frame`: 无栈帧

  编译器不会为函数创建栈帧，这样可以减少函数调用的开销。<br>
  如果函数中变量过多，寄存器不足，会导致编译错误。

### 多态

- `@vitural`: 声明函数为虚函数，用于实现多态。<br>
  也可以标记类为虚类，其中的虚函数均不可被定义而应该被其派生类定义。
- `@override`: 声明函数为重写函数，用于实现多态。<br>
  这要求基类中必须有对应的虚函数。
- `@final`: 声明函数为最终函数，禁止重写。

---

- `@const`: 声明函数为常函数，表明函数不依赖也不会修改任何外部状态。<br>
  *<span style="color:orange">warning</span>* 当一个函数可以为常函数但程序员未在声明中显式写明时
- `@pure`: 声明函数为纯函数，表明函数不依赖任何外部状态（除了传入指针指向的内存）也不会修改任何外部状态。<br>
  *<span style="color:orange">warning</span>* 当一个函数可以为纯函数但程序员未在声明中显式写明时
  > 注意：常量表达式不算外部状态
- `@noreturn`: 声明函数不会返回。
- `@rettwice`: 声明函数会返回两次。

---

- `@nonull(参数名列表)`: 声明函数的参数不会为 null。参数名列表是一个逗号分隔的参数名称列表。
- `@nonull`: 声明函数的所有指针参数不会为 null。
- `@nullable`: 在使用 `@nonull` 的情况下声明函数的某个参数可能为 null。
- `@checkparam`：在函数执行前对参数进行检查而不是相信程序员。

---

- `@constexpr`: 声明函数为编译时常量表达式，可以在编译时求值。
- `@inline`: 强制内联函数，使函数在调用处展开，而不是 C 那样的建议内联。<br>
  inline 的函数不能被导出。<br>
  <span style="color:purple">无特殊需求不应该使用</span>
- `@threadlocal`: 声明变量为线程局部变量，每个线程都会有一份独立的副本。
- `@restrict(参数名或序号列表)`: 声明函数的参数是限定指针，指针之间没有重叠。

  ```lumos
  @restrict(1, 2)
  fn memcpy(void* dst, void* src, size_t n) {
    实现
  }
  ```

## 变量属性

- `@align(对齐数)`: 声明变量按照指定的对齐数对齐。
- `@static`: 修饰函数中的变量，效果同 C语言
- `@thread`: 变量为每个线程独有，在函数中使用时同时自动设置变量为 `@static`
- `@register`: 强制变量存储在寄存器中，而不是内存中。<br>
  <span style="color:purple">无特殊需求不应该使用</span>

### 示例

```lumos
@const // 输入相同得到相同输出
fn square(int x) -> int {
  return x * x;
}

@inline // 强制内联
fn add(int a, int b) = a + b;

@hidden
fn helper(int x) -> int {
  实现
}
```
