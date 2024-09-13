#pragma once

#include <lumos/base.hpp>

enum class Attr : u32 {
  Static,
  Inline,
};

// 抽象语法树中节点的类型
namespace lumos::ast {

__Pclass__(AST);   //. 语法树节点基类
__Pclass__(Named); //. 有名称的语法树节点基类

// --------------------------------------------------
//; AST Named

__Pclass__(Type);      //. AST   -> 数据类型
__Pclass__(Evaluable); //. AST   -> 可求值对象
__Pclass__(BaseStat);  //. AST   -> 语句
__Pclass__(Container); //. Named -> 容器

// --------------------------------------------------
//; Evaluable

__Pclass__(FunctionCall);   //. Evaluable -> 函数调用
__Pclass__(Literal);        //. Evaluable -> 字面量 或 常量表达式
__Pclass__(SymbolRef);      //. Evaluable -> 对某个名称的引用
__Pclass__(Operation);      //. Evaluable -> 运算
__Pclass__(EvaluableBlock); //. Evaluable -> 可求值代码块
__Pclass__(Function);       //. Evaluable -> 函数

// --------------------------------------------------
//; Type

__Pclass__(PtrType);    //. Type ->
__Pclass__(ArrayType);  //. Type ->
__Pclass__(RefCntType); //. Type ->
__Pclass__(InnerType);  //. Type ->

__Pclass__(IntType); //. InnerType ->
__Pclass__(PtrType); //. InnerType ->
__Pclass__(PtrType); //. InnerType ->
__Pclass__(PtrType); //. InnerType ->

// --------------------------------------------------

__Pclass__(Variable); //. Named -> 变量

__Pclass__(Root); //. Container -> 根节点

__Pclass__(Exportable); //. Named -> 可导出的

__Pclass__(SymbolRef); // 对某个名称的引用

__Pclass__(NamedType); // 数据类型

__Pclass__(Block);     // 代码块
__Pclass__(Namespace); // 命名空间
__Pclass__(Struct);    // 结构体
__Pclass__(Class);     // 类

__Pclass__(Statement); //. BaseStat -> 单值语句
__Pclass__(IfStat);    //. BaseStat -> if 语句
__Pclass__(WhileStat); //. BaseStat -> while 语句
__Pclass__(ForStat);   //. BaseStat -> for 语句

__Pclass__(FunctionCall); // 函数调用

__Pclass__(String); // 字符串字面量
__Pclass__(Char);   // 字符字面量
__Pclass__(Number); // 数字字面量

__Pclass__(Operator); // 运算符

} // namespace lumos::ast
