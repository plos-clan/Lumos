#pragma once

#include "../type.hpp"

namespace lumos::ast {

//* ----------------------------------------------------------------------------------------------------
//; Expr
//* ----------------------------------------------------------------------------------------------------

class Expr : public AST {
public:
  PType type; // 该节点求值的类型

  Expr() : type(null) {}
};

// extern VoidType void_type;

class FunctionParam : public AST {
  Vector<Variable *> a;
};

class Operator : public Named {};

class Operation : public Expr {
  Operator *op;  // 要进行的运算
  Expr     *lhs; // 左侧的操作数
  Expr     *rhs; // 右侧的操作数
};

} // namespace lumos::ast
