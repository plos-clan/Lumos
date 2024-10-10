
# 类

## 重载类型转换

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

```lumos
impl MyInt {

fn as int {
    return value;
}

} // impl MyInt
```
