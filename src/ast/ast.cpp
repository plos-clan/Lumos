#include <lumos.hpp>

namespace lumos::ast {

auto operator<<(ostream &os, const AST &ast) -> ostream & {
  ast.print_to(os, 0);
  return os;
}

void AST::print_to(ostream &os, i32 indent) const {
  for (i32 i = 0; i < indent * 2; i++) {
    os.put(' ');
  }
  print_to(os);
  os << endl;
  print_children_to(os);
}

//* ----------------------------------------------------------------------------------------------------
//; Named
//* ----------------------------------------------------------------------------------------------------

Named::Named(Container *parent) : root(parent ? parent->root : null), parent(parent) {}

Named::Named(Container *parent, str name)
    : root(parent ? parent->root : null), parent(parent), name(std::move(name)) {
  if (parent == null) throw Error("Named 构造时 parent 不能为 null");
}

auto Named::mangling() -> str {
  osstream os("$$");
  mangling(os, mangling_style);
  return os.str();
}

auto Named::mangling(Mangling style) -> str {
  osstream os("$$");
  mangling(os, style);
  return os.str();
}

// 空的
void Named::mangling(std::ostream &os, Mangling style) {}

auto Named::print_to(ostream &os) const -> void {
  os << "<named " + name + "> If you see this node printed, please check the code.";
}

//* ----------------------------------------------------------------------------------------------------
//;
//* ----------------------------------------------------------------------------------------------------

auto Type::tostr() const -> str {}

RefCntType::RefCntType(Type *type) : type(type) {}

auto RefCntType::tostr() const -> str {
  return type->tostr() + "!";
}

class Operator : public Named {};

class Operation : public AST {
  Operator  *op;  // 要进行的运算
  Evaluable *lhs; // 左侧的操作数
  Evaluable *rhs; // 右侧的操作数
};

} // namespace lumos::ast
