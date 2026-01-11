
# 类 {#title}

## 声明 {#declaration}

在 Lumos 中，我们可以使用以下方式声明一个名为 MyClass 的类：

```lumos
class MyClass; // 声明一个类
```

## 定义 {#definition}

具名类（注意无需结尾的分号，也不能同时声明变量）

```lumos
class MyClass {
  省略实现
} // class MyClass
```

匿名类（必须同时声明变量）

```lumos
class {
  省略实现
} var my_var; // 定义一个匿名类并同时声明变量
```

## 零大小类型 (ZST) {#zst}

在 Lumos 中，不包含任何成员变量的类或结构体是不占空间的（Zero-sized）。这对于泛型编程和元编程非常有用。

```lumos
class Empty {};
val e = Empty{};
// sizeof(e) 为 0
```

## 初始化 {#init}

### 构造函数初始化 {#init-init}

### 变量列表初始化 {#list-init}

在 Lumos 中，我们可以使用以下方式初始化一个类：

```lumos
MyClass my_var1 = ${.a=1, .b=2, .c=3};
val my_var2 = MyClass${.a=1, .b=2, .c=3};
```

支持尾随逗号：`${.a=1, .b=2,}`。

需要类有以下定义：

```lumos
class MyClass {
  @public:
    i32 a, b, c;
    act initvar -> unit @default;
} // class MyClass
```

当然这边属性修饰 `@default` 加在前后都是可以的：

```lumos
@default act initvar -> unit;
act initvar -> unit @default;
```

默认情况下，如果一个类没有任何构造函数，那么它将允许变量列表初始化。  
如果有构造函数，则需要手动启用。

## 访问控制 {#access-control}

在 Lumos 中，我们可以使用 `@public`、`@protected` 和 `@private` 修饰符来控制类的访问权限。

```lumos
class MyClass {
  @public:
    i32 a;
  @protected:
    i32 b;
  @private:
    i32 c;
} // class MyClass
```

修饰符后加冒号表示这个修饰符之后的所有成员都会被修饰，建议访问控制修饰符加上冒号，不过也可以不加冒号。

```lumos
class MyClass {
  @public    i32 a;
  @protected i32 b;
  @private   i32 c;
} // class MyClass
```

你必须显式地写明使用哪个修饰符，否则编译器将会报错。

```lumos
class MyClass {
    i32 a; // 这会导致一个报错
} // class MyClass
```

```lumos
class MyClass {
    @private i32 a;
    i32 b; // 这会导致一个报错
} // class MyClass
```

## 重载类型转换 {#type-cast}

在 Lumos 中，我们可以使用 `def as 类型` 来重载类型转换。

```lumos
def as 类型;
def as 类型 {
  函数体
}
```

```lumos
class MyInt {
  @private:
    i32 value;

  @public:
    act MyInt(i32 value) : \var> value(value) {}

    def as i32 -> i32 {
        return value;
    }
} // class MyInt
```

也可以在类外定义：

```lumos
def MyInt.as i32 -> i32 {
    return value;
}
```

也可以省去每个函数前都写 `MyInt.` 的麻烦，使用 `impl` 关键字：

```lumos
impl MyInt {

def as i32 -> i32 {
    return value;
}

} // impl MyInt
```

## 弱化的类：结构体 {#struct-as-class}

在 Lumos 中，结构体是一个有功能限制的类，它没有一些高级特性。

**结构体有的特性：**

- 成员变量
- 成员函数
- 构造函数
- 析构函数
- 类型转换
- 继承

**结构体没有的特性：**

- 虚函数
- 多态
- 访问控制

结构体可以被类继承，但类无法被结构体继承。

### 如何选择使用结构体还是类 {#choice-usage-struct}

当你需要组织一个简单的数据时（比如红黑树中的节点），你应该使用结构体。

当你需要对复杂数据进行管理时（比如红黑树本身），你应该使用类。

也就是我们应该声明：

```lumos
struct RBTreeNode;
class RBTree;
```

或者直接在类中定义结构体：

```lumos
class RBTree {
  @private:
    struct Node {
        省略实现
    }
  
  省略实现
}
```

---

相关内容：对象模型见 [对象](object.md#title)。
