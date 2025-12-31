
# 输入输出 `std.io`

`std` 中的默认 `using`：

```lumos
using \type> io.\act> print;
using \type> io.\act> println;
using \type> io.\act> scan;
using \type> io.\act> scanln;
```

```lumos
print("Hello world!");   // 输出不换行
println("Hello world!"); // 输出并换行

int a;
print("Please input a number: ");
scan(a); // 输入一个整数
```
