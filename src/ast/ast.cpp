#include <lumos.hpp>

namespace lumos::ast {

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
  POperator  op;    // 要进行的运算
  PEvaluable left;  // 左侧的操作数
  PEvaluable right; // 右侧的操作数
};

} // namespace lumos::ast
