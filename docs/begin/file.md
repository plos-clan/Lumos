
# 文件

Lumos 要求所有数据都以 UTF-8 编码，文件名区分大小写。
<span style="color:green">但当文件或目录名仅有大小写不同时会报警告。</span>

## 文件与扩展名

- Lumos 源代码文件：`.lm`
- Lumos 头文件：`.lh`
- Lumos 项目配置文件：`lumos.yaml` / `lumos.json` / `lumos.toml`（三选一，且只能存在一个）

## 工具链

- `lumos` Lumos 脚本解释器
- `lumosc` Lumos 编译器
- `lumosl` Lumos 链接器
- `lumosb` Lumos 项目构建器
- `lumosd` Lumos 语言服务器
- `lumosp` Lumos 包管理器

### 编译器

### 链接器

### 项目构建器

可以根据单个 lumos 源代码文件(`.lm`)或 lumos 项目配置文件(`lumos.yaml`/`lumos.json`/`lumos.toml`，三选一)自动配置项目并生成可执行文件或库。

### 语言服务器

### 包管理器

类似于 apt 的包管理器，可以安装、卸载、更新、搜索、列出、显示包信息等。
