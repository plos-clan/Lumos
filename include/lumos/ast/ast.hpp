#pragma once
#include "../mangling.hpp"

namespace lumos::ast {

__ERRORIMPL__("AST");

//* ----------------------------------------------------------------------------------------------------
//; decl
//* ----------------------------------------------------------------------------------------------------

Pclass(AST);   //. 语法树节点基类
Pclass(Named); //. 有名称的语法树节点基类

// --------------------------------------------------
//; AST Named

Pclass(Type);      //. AST   -> 数据类型
Pclass(Expr);      //. AST   -> 可求值对象
Pclass(BaseStat);  //. AST   -> 语句
Pclass(Container); //. Named -> 容器

// --------------------------------------------------
//; Expr

Pclass(FunctionCall); //. Expr -> 函数调用
Pclass(Literal);      //. Expr -> 字面量 或 常量表达式
Pclass(SymbolRef);    //. Expr -> 对某个名称的引用
Pclass(Operation);    //. Expr -> 运算
Pclass(ExprBlock);    //. Expr -> 可求值代码块
Pclass(Function);     //. Expr -> 函数
Pclass(ArrayAccess);  //. Expr -> 函数

// --------------------------------------------------
//; Type

Pclass(PtrType);    //. Type ->
Pclass(ArrayType);  //. Type ->
Pclass(RefCntType); //. Type ->
Pclass(InnerType);  //. Type ->

Pclass(IntType); //. InnerType ->
Pclass(PtrType); //. InnerType ->
Pclass(PtrType); //. InnerType ->
Pclass(PtrType); //. InnerType ->

Pclass(Struct); //. Type ->
Pclass(Class);  //. Type ->

// --------------------------------------------------

Pclass(Variable); //. Named -> 变量

Pclass(Root); //. Container -> 根节点

Pclass(Exportable); //. Named -> 可导出的

Pclass(NamedType); // 数据类型

Pclass(Block);     // 代码块
Pclass(Namespace); // 命名空间
Pclass(Struct);    // 结构体
Pclass(Class);     // 类

Pclass(Statement); //. BaseStat -> 单值语句
Pclass(IfStat);    //. BaseStat -> if 语句
Pclass(WhileStat); //. BaseStat -> while 语句
Pclass(ForStat);   //. BaseStat -> for 语句

Pclass(FunctionCall); // 函数调用

Pclass(String); // 字符串字面量
Pclass(Char);   // 字符字面量
Pclass(Number); // 数字字面量

Pclass(Operator); // 运算符

//* ----------------------------------------------------------------------------------------------------
//; AST Named
//* ----------------------------------------------------------------------------------------------------

class Object {
public:
  explicit virtual operator std::string() const;
};

class AST {
public:
  u32 id; // 对象的 ID
  AST()          = default;
  virtual ~AST() = default;

  // 输出 直接调用 print_to 不应该修改
  friend auto operator<<(ostream &os, const AST &ast) -> ostream &;

  // debug 用
  // 检查对象是否有错误
  // 有错误直接抛出异常
  virtual void check() const;

  virtual void gencode() const;      // 生成代码
  virtual void eval(ENV &env) const; // 求值、运行

protected:
  virtual void print_to(ostream &os) const;
  virtual void print_children_to(ostream &os, i32 indent = 0) const;

private:
  void print_to(ostream &os, i32 indent) const;
};

// --------------------------------------------------

/**
 *\brief 有名称对象（基类）
 *
 */
class Named : public AST {
public:                     // 根元素的以下两个属性为 null
  Root      *root   = null; // 储存根元素
  Container *parent = null; // 储存父元素
  const str  name;          // 节点本身的名称

  // 对于内部节点导出的，该值表示
  //   该节点内部的元素是否导出
  // 对于节点本身导出的，该值表示
  //   该节点导出时名称重整的策略
  Mangling mangling_style = Mangling::none;

  Named(Container *parent, str name);
  ~Named() override = default;

  // 获取该 AST 节点的重整名称
  // style: 名称重整的风格，没有就按照 mangling_style 成员的值
  auto         mangling() -> str;               // 不应该更改
  auto         mangling(Mangling style) -> str; // 不应该更改
  virtual void mangling(strbuilder &sb, Mangling style);

protected:
  void print_to(ostream &os) const override;
};

} // namespace lumos::ast

namespace lumos {
pusing(ast, AST);
}
