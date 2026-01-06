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

void AST::print_to(ostream &os) const {
  os << "<AST> // what the fuck!";
}

void AST::print_children_to(ostream &os, i32 indent) const {
  // 无
}

void AST::check() const {
  throw Fail("AST::check() 不能被直接调用");
}

void AST::gencode() const {
  throw Fail("AST::gencode() 不能被直接调用");
}

void AST::eval(ENV &env) const {
  throw Fail("AST::eval() 不能被直接调用");
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
  strbuilder sb = "$$";
  mangling(sb, mangling_style);
  return sb.str();
}

auto Named::mangling(Mangling style) -> str {
  strbuilder sb = "$$";
  mangling(sb, style);
  return sb.str();
}

void Named::mangling(strbuilder &sb, Mangling style) {
  throw Fail("Named::mangling 不能被直接调用");
}

void Named::print_to(ostream &os) const {
  os << "<named " << name << "> // what the fuck!";
}

//* ----------------------------------------------------------------------------------------------------
//;
//* ----------------------------------------------------------------------------------------------------

auto Type::tostr() const -> str {
  return "";
}

auto PtrType::tostr() const -> str {
  return "[" + type->tostr() + "]" + (nullable ? "?" : "");
}

auto RefType::tostr() const -> str {
  return "&" + type->tostr();
}

RefCntType::RefCntType(Type *type) : type(type) {}

auto RefCntType::tostr() const -> str {
  return type->tostr() + "!";
}

} // namespace lumos::ast
