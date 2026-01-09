# 声明 (Declaration)

本页定义了顶层声明及各类定义结构。

## 顶层声明 `Decl`

```rust
pub enum Decl {
    /// 函数定义
    Function(FunctionDecl),
    /// 变量定义
    Variable(VariableDecl),
    /// 类定义
    Class(ClassDecl),
    /// 结构体定义
    Struct(StructDecl),
    /// 枚举定义
    Enum(EnumDecl),
    /// 模块/命名空间定义
    Module(ModuleDecl),
    /// 权限别名定义 (permission a.b aka c)
    PermissionAlias {
        path: Vec<String>,
        alias: String,
    },
    /// 模块配额声明 (use mod with [perm])
    Using {
        module: String,
        permissions: Vec<String>,
    },
    /// 类型别名 (type NewName = OldType)
    TypeAlias {
        name: String,
        target: TypeRef,
    },
}
```

## 函数声明 `FunctionDecl`

```rust
pub struct FunctionDecl {
    pub purity: Purity,
    pub modifiers: Vec<Modifier>,
    pub member_mod: Option<MemberModifier>, // 仅用于成员函数
    pub name: String,
    pub captures: Vec<Capture>,             // 捕获列表
    pub params: Vec<Param>,                 // 参数列表
    pub return_type: Option<TypeRef>,       // 返回类型 (None 表示推断)
    pub non_returning: bool,                // 是否带有 - 标记 (不返回)
    pub where_clause: Option<Expr>,         // 函数级约束
    pub permissions: Vec<String>,           // 所需权限 (仅 Act)
    pub yields: Vec<String>,                // 传播权限
    pub body: Option<Block>,                // 函数体 (None 表示仅声明)
}
```

## 变量声明 `VariableDecl`

```rust
pub struct VariableDecl {
    pub modifier: VarModifier,
    pub strategy: InitStrategy,
    pub name: String,
    pub r#type: Option<TypeRef>,
    pub init_expr: Option<Expr>,
}
```

## 类与结构体声明

```rust
pub struct ClassDecl {
    pub name: String,
    pub generics: Vec<String>,
    pub bases: Vec<TypeRef>,      // 基类/接口
    pub members: Vec<Member>,
}

pub struct StructDecl {
    pub name: String,
    pub generics: Vec<String>,
    pub members: Vec<Member>,
}

pub struct Member {
    pub access: AccessModifier,
    pub kind: MemberKind,
}

pub enum MemberKind {
    Field(VariableDecl),
    Method(FunctionDecl),
    Constructor(FunctionDecl),
    Destructor(FunctionDecl),
}
```

## 枚举声明 `EnumDecl`

```rust
pub struct EnumDecl {
    pub name: String,
    pub variants: Vec<EnumVariant>,
    pub is_measure: bool, // 是否作为计量单位定义 (as measure)
}

pub struct EnumVariant {
    pub name: String,
    pub value: Option<Expr>,
}
```

## 模块声明 `ModuleDecl`

```rust
pub struct ModuleDecl {
    pub name: String,
    pub decls: Vec<Decl>,
}
```

---

相关内容：声明语法见 [语法/函数](../grammar/function.md#functions)。
