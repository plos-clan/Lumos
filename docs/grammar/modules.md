
# 模块

## 导入模块

使用 `using` 来导入模块，模块中的内容将会被导入到当前命名空间中。

```lumos
using "std";
```

这样会导致一些问题，比如向当前命名空间导入了一些不需要或不应该导入的内容。<br>
所以 Lumos 提供了一种写法，可以导入模块内容到指定的命名空间中。

```lumos
using "std" -> std; // 导入 std 模块到 std 命名空间
                    // std 命名空间可以不存在
```

上面的写法相当于

```lumos
namespace std {
    using "std";
}
```

导入模块后可以 using 需要的内容。

```lumos
using "std" -> std;
using std.println;
```

## 构建模块

Lumos 的模块是一个配置文件，一个简单的架构为

```files
my_model/
  main.lh
  main.lm
  lumos.yaml
```

`lumos.yaml` 的基本形式为

```yaml
- name: my_model
- using: # 依赖的模块（不允许循环依赖）
  - std  # 标准库
- decl:  # 声明所在的文件
  - main # 查找时忽略 .lm 文件，除非使用完整文件名
- impl:  # 定义所在的文件
  - main # 查找时忽略 .lh 文件，除非使用完整文件名
```
