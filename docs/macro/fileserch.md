
# 文件查找规则

当你给出一个文件名（如 `xxx`）时，Lumos 将按照以下方式解析文件名：

<span style="color:green">部分文件查找可能会忽略某些步骤，但完整文件名一定不会被忽略。</span>

1. 完整文件名，查找工作目录下是否存在路径 `xxx`
   1. 为文件则终止，为目录则继续
   2. 查找 `xxx/lumos.yaml` / `xxx/lumos.json` / `xxx/lumos.toml` 文件
      1. 若存在多个配置文件，抛出错误
      2. 若仅存在一个，找到则终止
   3. 查找 `xxx/main.lh` 文件，找到则终止
   4. 查找 `xxx/main.lm` 文件，找到则终止
2. 配置文件名，查找工作目录下是否存在路径 `xxx.yaml` / `xxx.json` / `xxx.toml`
   1. 若存在多个配置文件，抛出错误
   2. 若仅存在一个，为文件则终止
3. 头文件名，查找工作目录下是否存在路径 `xxx.lh`
   1. 为文件，找到则终止
4. 源文件名，查找工作目录下是否存在路径 `xxx.lm`
   1. 为文件，找到则终止
5. 在默认路径下查找（再次执行 1 至 4 步）
6. 抛出错误：找不到文件 `xxx`

*<span style="color:orange">warning</span>: <span style="color:darkcyan">include 一个模块时</span>*
