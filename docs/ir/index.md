# Lumos 中间表示 (LIR) 设计

Lumos IR (LIR) 是一种基于 **SSA (Static Single Assignment)** 形式的线性中间表示，旨在连接高级 AST 和低级机器码（如 LLVM IR 或汇编）。

## 设计目标

- **效应感知**: 显式表示副作用和权限校验。
- **纯度优化**: 利用 `def` 和 `fun` 的特性进行更激进的常量折叠和死代码消除。
- **内存安全**: 承载 `imv` (物理不可变) 和 `late`/`lazy` 的运行时语义。

## 核心结构

### 模块 (Module)

包含全局变量、函数定义和类型元数据。

### 函数 (Function)

由一组 **基本块 (Basic Blocks)** 组成。每个函数记录其纯度等级和所需的权限集。

### 基本块 (Basic Block)

包含一系列指令，并以 **终结符 (Terminator)** 结尾。

## 指令集

### 基础运算

- `add`, `sub`, `mul`, `div`, `rem`
- `and`, `or`, `xor`, `shl`, `shr`
- `icmp`, `fcmp` (比较)

### 内存操作

- `alloca`: 栈分配。
- `load` / `store`: 内存读写。
- `getelementptr`: 计算成员地址。

### 流程控制 (Terminators)

- `br`: 无条件跳转。
- `cond_br`: 条件跳转。
- `ret`: 返回。
- `switch`: 多分支跳转。

### Lumos 特色指令

#### 效应调用 `eff_call`

```lir
%res = eff_call @func(%arg1, ...), perms [io.out]
```

显式标注调用所需的权限。在 IR 验证阶段，若当前函数权限集不包含 `perms`，则视为非法。

#### 懒加载处理 `lazy_load`

```lir
%val = lazy_load @lazy_metadata, %ptr
```

封装了线程安全的双重检查锁定逻辑。如果 `%ptr` 未初始化，则调用关联的初始化闭包。

#### 静态假设 `assume`

```lir
assume %cond
```

向优化器传递信息：在此处 `%cond` 必为真。优化器可以据此消除分支或简化运算。

#### 纯度标记 `purity_scope`

标记一个区域为 `logic_pure`。在该区域内，优化器可以假设外部全局状态不会改变。

## 效应系统在 IR 中的实现

LIR 使用 **权限掩码 (Permission Mask)** 或 **能力令牌 (Capability Tokens)** 来跟踪副作用。

1. **静态校验**: 编译器在 LIR 层面进行数据流分析，确保所有 `eff_call` 都在合法的权限上下文中。
2. **权限传播**: 处理 `yields` 语义，在函数返回后更新调用方的权限状态。

## 降级路径 (Lowering)

1. **AST -> LIR**: 进行作用域展开、Lambda 提升、`lazy` 逻辑生成。
2. **LIR 优化**: 纯度相关的内联、SSA 优化、死代码消除。
3. **LIR -> LLVM IR / Machine Code**: 将 LIR 指令映射到目标平台的指令。
