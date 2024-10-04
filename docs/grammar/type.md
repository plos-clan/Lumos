
# 类型

内置类型的信息见 [词法/内置类型](../lexical/type.md) 。

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
        return void;s
    }
    return arg;
}

fn main() {
    var value = foo(10);
    switch value {
    int => println("返回值是 int: ", value);
    void => println("返回值是 void");
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
    int => println("返回值是 int: ", value);
    void => println("返回值是 void");
    }
}
```
