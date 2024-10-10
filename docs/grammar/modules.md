
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

`main.lh` 和 `main.lm` 的基本形式为

```lumos
fn my_func();
```

```lumos
fn my_func() {
    println("Hello, world!");
}
```

`lumos.yaml` 的基本形式为

```yaml
# 基本信息（可选）
name: my_model # 模块名（默认为所在文件夹名）
version: 1.0.0 # 模块版本号（默认为 1.0.0）
std: 1.0.0     # 使用的语言标准版本（默认为最新）
target: x86_64 # 目标平台（默认为当前平台）
# 附加信息（可选）
author: xxx      # 作者
email: xxx       # 联系邮箱
license: xxx     # 许可证
description: xxx # 描述
url: xxx         # 项目地址
type: xxx        # 模块类型
tags:            # 标签
  - xxx
  - xxx
issues: xxx      # 问题地址
sponsor: xxx     # 赞助商
# 编译选项（可选）
flags:
  debug:
    - -O0 -g
  release:
    - -O2
# 模块依赖（可选）
dependencies: # 不允许循环依赖
  - std       # 标准库
  - xxx       # 其他模块
using:             # 实现文件中默认导入的模块声明
  - "std  -> ::" # 标准库
  - "self -> ::" # 当前模块
# 模块本体
decl:    # 声明所在的文件
  - main # 查找时忽略 .lm 文件，除非使用完整文件名
impl:    # 实现所在的文件
  - main # 查找时忽略 .lh 文件，除非使用完整文件名
```

- `name` 模块名，不能包含特殊字符，不能为 self。
- `version` 模块版本号，格式为 `x.y.z`。
- `std` 使用的语言标准版本。
- `target` 目标平台。
- `author` 作者。*<span style="color:green">人员信息或人员信息列表</span>*
- `email` 联系邮箱。*<span style="color:green">字符串或字符串列表</span>*
- `license` 许可证。*<span style="color:green">字符串</span>*
- `description` 描述。*<span style="color:green">字符串</span>*
- `url` 项目地址。*<span style="color:green">url 字符串</span>*
- `type` 模块类型。*<span style="color:green">字符串</span>*
- `tags` 标签。*<span style="color:green">字符串列表</span>*
- `issues` 问题地址。*<span style="color:green">url 字符串</span>*
- `sponsor` 赞助商。*<span style="color:green">人员信息或人员信息列表*</span>
- `flags` 编译选项。
- `dependencies` 模块依赖。*<span style="color:green">字符串列表</span>*
- `using` 实现文件中默认导入的模块声明。*<span style="color:green">字符串列表</span>*
- `decl` 声明所在的文件。*<span style="color:green">字符串列表</span>*
- `impl` 实现所在的文件。*<span style="color:green">字符串列表</span>*

***人员信息***可以是一个字符串（表示名称），也可以是一个对象。

```yaml
author:
  - xxx
  - yyy
  - name: zzz # 名称（必须）
    # 以下为可选项
    email: zzz  # 联系邮箱
    avatar: zzz # 头像地址
    url: zzz    # 个人主页
```
