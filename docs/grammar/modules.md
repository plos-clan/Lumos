
# 模块 {#title}

## 导入模块 {#import}

使用 `use` 来导入模块，模块中的内容将会被导入到对应的命名空间中。

```lumos
use "std";
```

*对应的命名空间* 指配置文件中指定的模块名，如果没有指定则为模块所在的文件夹名。

Lumos 提供了一种写法，可以导入模块内容到指定的命名空间中。

```lumos
use "std" -> abc; // 导入 std 模块到 abc 命名空间
                    // abc 命名空间可以不存在
```

导入模块后可以 use 需要的内容。

```lumos
use "std";
use std.println;
```

当然也可以导入到根命名空间：

```lumos
use "std" -> ::;
```

但这样会导致一些问题，比如向当前命名空间导入了一些不需要或不应该导入的内容。

## 构建模块 {#module-build}

Lumos 的模块由一个配置文件（`lumos.yaml`/`lumos.json`/`lumos.toml`，三选一）定义，一个简单的结构为

```files
my_model/
  main.lh
  main.lm
  lumos.yaml
```

同一目录下只允许存在一个配置文件；若同时存在 `lumos.yaml`/`lumos.json`/`lumos.toml`，将直接报错。三种格式共享同一配置树和键名。示例统一以 `lumos.yaml` 展示。

```yaml
name: my_model
space: my_model
dependencies:
  - std
use:
  - "self -> ::"
decl: "**.lh"
impl: "**.lm"
```

### 源代码 {#source-code}

`main.lh` 和 `main.lm` 的基本形式为

```lumos
act my_func() -> unit;
```

```lumos
act[io.out] my_func() -> unit {
    println("Hello, world!");
}
```

### 配置文件 {#config-file}

配置文件的基本形式为

```yaml
# 基本信息（可选）
name: my_model  # 模块的显示名（默认为所在文件夹名）
name_ascii: xxx # 模块显示名的 ASCII 版本（默认为 name 本身删除非 ASCII 字符）
space: xxx      # 模块的命名空间（默认为根空间）
version: 1.0.0  # 模块版本号（默认为 0.0.0）
std: 1.0.0      # 使用的语言标准版本（默认为最新）
target: x86_64  # 目标平台（默认为当前平台）
type: xxx       # 模块类型
subproj: false  # 是否为子模块

# 附加信息（可选）
author: xxx      # 作者
email: xxx       # 联系邮箱
license: xxx     # 许可证
description: xxx # 描述
repo: xxx        # 仓库地址
url: xxx         # 项目官网
doc: xxx         # 文档地址
tags:            # 标签
  - xxx
  - xxx
issue: xxx       # 问题地址
sponsor: xxx     # 赞助商
fork: xxx        # 派生自
icon: xxx        # 图标地址 (本地或远程)

# 编译选项（可选）
flags:
  debug: >
    -O0 -g
  release: >
    -O2

# 模块依赖（可选）
dependencies: # 不允许循环依赖
  - std       # 标准库
  - xxx       # 其他模块
use:             # 实现文件中默认导入的模块声明
  - "std  -> ::" # 标准库
  - "self -> ::" # 当前模块

# 模块本体
decl:    # 声明所在的文件（默认为 **.lh）
  - main # 查找时忽略 .lm 文件，除非使用完整文件名
impl:    # 实现所在的文件（默认为 **.lm）
  - main # 查找时忽略 .lh 文件，除非使用完整文件名
```

- `name` 模块的显示名。
- `name_ascii` 模块显示名的 ASCII 版本，不能包含特殊字符。
- `version` 模块版本号，格式为 `x.y.z`。
- `std` 使用的语言标准版本。
- `target` 目标平台。
- `author` 作者。*<span style="color:green">人员信息或人员信息列表</span>*
- `email` 项目公共联系邮箱。*<span style="color:green">字符串或字符串列表</span>*
- `license` 许可证。*<span style="color:green">字符串</span>*
- `description` 描述。*<span style="color:green">字符串</span>*
- `url` 项目地址。*<span style="color:green">url 字符串</span>*
- `type` 模块类型。*<span style="color:green">字符串</span>*
- `tags` 标签。*<span style="color:green">字符串列表</span>*
- `issue` 问题地址。*<span style="color:green">url 字符串</span>*
- `sponsor` 赞助商。*<span style="color:green">人员信息或人员信息列表*</span>
- `flags` 编译选项。
- `dependencies` 模块依赖。*<span style="color:green">字符串列表</span>*
- `use` 实现文件中默认导入的模块声明。*<span style="color:green">字符串列表</span>*
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

可以使用更复杂的目录结构，比如模块中包含子模块：

```files
my_model/
  my_submodel/
    main.lh
    main.lm
    lumos.yaml
  main.lh
  main.lm
  lumos.yaml
```

此时你可以在配置文件中

```yaml
use:
  - ".my_submodel -> submodel"
```

#### type {#type}

- `app` 应用模块，产生可执行文件。
- `lib` 库模块，产生库文件。
- `meta` 元模块，本身不产生库或可执行文件，但可以包含子模块。

## 模块宏 {#macros}

`ismodelimpl` 宏用于判断当前文件是否被当前模块实现文件包含或导入。

```lumos
#if ismodelimpl
// 代码
#endif
```

`isfileimpl` 宏用于判断当前文件是否被对应的实现文件包含。  
*如 `xxx.lh` 和 `xxx.lm`*

```lumos
#if isfileimpl
// 代码
#endif
```

---

相关内容：权限系统见 [副作用权限系统](permission.md#title)。
