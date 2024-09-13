#pragma once

#include "../mangling.hpp"

namespace lumos::ast {

// 用户输入错误
class Error : public ::Error {
public:
  explicit Error(const str &msg) : ::Error("AST: " + msg) {}
};

// 程序内部错误
class Fail : public ::Error {
public:
  explicit Fail(const str &msg) : ::Error("[fail] AST: " + msg) {}
};

//* ----------------------------------------------------------------------------------------------------
//; decl
//* ----------------------------------------------------------------------------------------------------

class AST;   //. 语法树节点基类
class Named; //. 有名称的语法树节点基类

// --------------------------------------------------
//; AST Named

class Type;      //. AST   -> 数据类型
class Evaluable; //. AST   -> 可求值对象
class BaseStat;  //. AST   -> 语句
class Container; //. Named -> 容器

// --------------------------------------------------
//; Evaluable

class FunctionCall;   //. Evaluable -> 函数调用
class Literal;        //. Evaluable -> 字面量 或 常量表达式
class SymbolRef;      //. Evaluable -> 对某个名称的引用
class Operation;      //. Evaluable -> 运算
class EvaluableBlock; //. Evaluable -> 可求值代码块
class Function;       //. Evaluable -> 函数

// --------------------------------------------------
//; Type

class PtrType;    //. Type ->
class ArrayType;  //. Type ->
class RefCntType; //. Type ->
class InnerType;  //. Type ->

class IntType; //. InnerType ->
class PtrType; //. InnerType ->
class PtrType; //. InnerType ->
class PtrType; //. InnerType ->

// --------------------------------------------------

class Variable; //. Named -> 变量

class Root; //. Container -> 根节点

class Exportable; //. Named -> 可导出的

class SymbolRef; // 对某个名称的引用

class NamedType; // 数据类型

class Block;     // 代码块
class Namespace; // 命名空间
class Struct;    // 结构体
class Class;     // 类

class Statement; //. BaseStat -> 单值语句
class IfStat;    //. BaseStat -> if 语句
class WhileStat; //. BaseStat -> while 语句
class ForStat;   //. BaseStat -> for 语句

class String; // 字符串字面量
class Char;   // 字符字面量
class Number; // 数字字面量

class Operator; // 运算符

//* ----------------------------------------------------------------------------------------------------
//; AST Named
//* ----------------------------------------------------------------------------------------------------

class AST {
public:
  u32 id; // 对象的 ID
  AST() {}
  virtual ~AST() = default;

  // 输出 直接调用 print_to 不应该修改
  friend auto operator<<(ostream &os, const AST &ast) -> ostream & {
    ast.print_to(os, 0);
    return os;
  }

  // debug 用
  // 检查对象是否有错误
  // 有错误直接抛出异常
  virtual void check() const {
    throw Fail("AST::check() 不应该被调用");
  }

protected:
  virtual auto print_to(ostream &os) const -> void;
  virtual auto print_children_to(ostream &os, i32 indent = 0) const -> void;

private:
  void print_to(ostream &os, i32 indent) const {
    for (i32 i = 0; i < indent * 2; i++) {
      os.put(' ');
    }
    print_to(os);
    os << endl;
    print_children_to(os);
  }
};

// --------------------------------------------------

// 不强制 Named 有名称
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

  explicit Named(Container *parent);
  Named(Container *parent, str name);
  ~Named() override = default;

  // 获取该 AST 节点的重整名称
  // style: 名称重整的风格，没有就按照 mangling_style 成员的值
  auto         mangling() -> str;               // 不应该更改
  auto         mangling(Mangling style) -> str; // 不应该更改
  virtual auto mangling(ostream &os, Mangling style) -> void;

protected:
  auto print_to(ostream &os) const -> void override;
};

} // namespace lumos::ast

namespace lumos {
using ast::AST;
}
