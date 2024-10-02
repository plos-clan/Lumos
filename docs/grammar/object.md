# 动态类型

Lumos 中可以使用动态类型，变量的类型在运行时确定，就像 js 那样。<br>
即变量可以在不同的时间点存储不同类型的值。<br>
<span style="color:purple">
当然我们不建议你在 Lumos 中过多使用动态类型，因为这会导致代码难以理解和维护。
</span>

!!! note "我知道你在想什么"
    `list() + map()` 不会得到 `"[object Object]"`，别试了。

```lumos
obj a = 10;  // 现在 a 为 i32 类型
a     = 0.1; // 现在 a 为 f32 类型
a     = "1"; // 现在 a 为 str 类型
```

对动态类型成员的访问都会被封装为对哈希表的访问。

```lumos
a.xxx(); // ((void (*)())a.members["xxx"])();
```

使用 `typeof` 运算符可以获取它的类型。<br>
使用 `typenameof` 运算符可以以字符串形式获取它的类型。

```lumos
obj a = 10;
println(typenameof(a)); // i32
```

如果不初始化动态类型对象，那么它的类型为 `void`，值为 `undefined`。

```lumos
obj a;         // 不初始化
a == undefined; // true
typeof(a);      // void
```

锁定动态类型对象的类型

```lumos
fn my_func(obj arg1 @lock-type) {
  arg1 = 1; // 此时 arg1 的类型并不会变为 `int`
            // 而是会调用 `arg1` 原类型的构造函数
}
```

!!! warning "优先考虑使用重载与模板"

    ```lumos
    @<typename T>
    fn my_func(T arg1) {
      arg1 = 1;
    }
    ```

## 实现

实际上就是将运算符进行封装。<br>
类似以下 C 代码。

```c
typedef struct object *object_t;
typedef object_t (*op1_t)(object_t *);
typedef object_t (*op2_t)(object_t *, object_t *);
typedef struct callbacks {
    op2_t add;
    op2_t sub;
    op2_t mul;
    op2_t div;
} *callbacks_t;
struct object {
  callbacks_t cbs;
};
```

```c
a = a.cbs->add(a, b);
```
