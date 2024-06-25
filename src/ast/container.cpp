#include "lumos/ast/ast/decl.hpp"
#include "type.hpp"
#include <lumos/ast.hpp>

namespace lumos::ast {

//* ----------------------------------------------------------------------------------------------------
//; Container
//* ----------------------------------------------------------------------------------------------------

Container::Container(Container *parent) : Named(parent) {}

Container::Container(Container *parent, str name) : Named(parent, std::move(name)) {}

auto Container::find(const str &name) const -> AST * {
  if (auto it = children.find(name); it != children.end()) return it->second;
  return parent != null ? parent->find(name) : null;
}

auto Container::append(Named *child) -> void {
  if (child == null) throw Error("请检查代码: Container::append 传入的 child 为 null");
  if (child->name.empty()) throw Error("请检查代码: Container::append 传入的 child 没有名称");
  children[child->name] = child;
}

//* ----------------------------------------------------------------------------------------------------
//; Block
//* ----------------------------------------------------------------------------------------------------

auto Block::append(Named *child) -> void {
  if (child == null) throw Error("请检查代码: Container::append 传入的 child 为 null");
  if (child->name.empty()) throw Error("请检查代码: Container::append 传入的 child 没有名称");
  children[child->name] = child;
}

auto Block::append(BaseStat *child) -> void {
  if (child == null) throw Error("请检查代码: Container::append 传入的 child 为 null");
  code.emplace_back(child);
}

auto Block::append(Block *child) -> void {
  if (child == null) throw Error("请检查代码: Container::append 传入的 child 为 null");
  code.emplace_back(child);
}

//* ----------------------------------------------------------------------------------------------------
//; Root
//* ----------------------------------------------------------------------------------------------------

auto Root::print_to(ostream &os) const -> void {
  os << "<root>";
}

//* ----------------------------------------------------------------------------------------------------
//; Class Struct
//* ----------------------------------------------------------------------------------------------------

auto Class::append(Named *child) -> void {
  if (child == null) throw Error("请检查代码: Class::append 传入的 child 为 null");
  if (child->name.empty()) throw Error("请检查代码: Class::append 传入的 child 没有名称");
  if (isinstance<Namespace>(child)) throw Error("不能将 namespace 插入 class 中");
  children[child->name] = child;
}

#define Error_CheckCode(why)

auto Struct::append(Named *child) -> void {
  if (child == null) throw Error("请检查代码: Struct::append 传入的 child 为 null");
  if (child->name.empty()) throw Error("请检查代码: Struct::append 传入的 child 没有名称");
  if (isinstance<Namespace>(child)) throw Error("不能将 namespace 插入 class 中");
  children[child->name] = child;
}

} // namespace lumos::ast
