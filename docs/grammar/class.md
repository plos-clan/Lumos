
# 类

## 初始化

### 构造函数初始化

### 变量列表初始化

在 Lumos 中，我们可以使用以下方式初始化一个类：

```lumos
MyClass my_var1 = {1, 2, 3};
var my_var2 = MyClass{1, 2, 3};
```

需要类有以下定义：

```lumos
class MyClass {
  @public:
    int a, b, c;
    fn initvar @default;
} // class MyClass
```

当然这边属性修饰 `@default` 加在前后都是可以的：

```lumos
@default fn initvar;
fn initvar @default;
```

默认情况下，如果一个类没有任何构造函数，那么它将允许变量列表初始化。<br>
如果有构造函数，则需要手动启用。

## 访问控制

在 Lumos 中，我们可以使用 `@public`、`@protected` 和 `@private` 修饰符来控制类的访问权限。

```lumos
class MyClass {
  @public:
    int a;
  @protected:
    int b;
  @private:
    int c;
} // class MyClass
```

修饰符后加冒号表示这个修饰符之后的所有成员都会被修饰，建议访问控制修饰符加上冒号，不过也可以不加冒号。

```lumos
class MyClass {
  @public int a;
  @protected int b;
  @private int c;
} // class MyClass
```

你必须显式地写明使用哪个修饰符，否则编译器将会报错。

```lumos
class MyClass {
    int a; // 这会导致一个报错
} // class MyClass
```

```lumos
class MyClass {
    @private int a;
    int b; // 这会导致一个报错
} // class MyClass
```

## 重载类型转换

在 Lumos 中，我们可以使用 `fn as 类型` 来重载类型转换。

```lumos
fn as 类型;
fn as 类型 {
  函数体
}
```

```lumos
class MyInt {
  @private:
    int value;

  @public:
    fn MyInt(int value) : \var> value(value) {}

    fn as int {
        return value;
    }
} // class MyInt
```

也可以在类外定义：

```lumos
fn MyInt.as int {
    return value;
}
```

也可以省去每个函数前都写 `MyInt.` 的麻烦，使用 `impl` 关键字：

```lumos
impl MyInt {

fn as int {
    return value;
}

} // impl MyInt
```

## 弱化的类：结构体

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

### 如何选择使用结构体还是类

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
