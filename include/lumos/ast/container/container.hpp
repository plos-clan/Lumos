#pragma once

#include "../ast.hpp"
#include "../statement.hpp"

namespace lumos::ast {

//* ----------------------------------------------------------------------------------------------------
//; Container
//* ----------------------------------------------------------------------------------------------------

// 不强制 Container 有名称
// 内部元素无法导出的 Container 在 mangling 函数中会抛出错误
// 例如 lambda 表达式
class Container : public Named {
public:
  ST<PNamed> children; // 子元素

  // 用于无名节点的构造
  explicit Container(Container *parent);
  // 用于有名节点的构造
  Container(Container *parent, str name);
  ~Container() override = default;

  // 从 Container 中搜索指定名称的节点
  // 该 Container 中搜不到就往上层搜
  virtual auto find(const str &name) const -> AST *;

  // child 由调用者分配，所有权转移到类
  virtual auto append(Named *child) -> void;
};

//* ----------------------------------------------------------------------------------------------------
//; 直接继承 Container 的类
//* ----------------------------------------------------------------------------------------------------

// 用于代码块
class Block : public Container, public BaseStat {
public:
  Vector<PBaseStat> code; // 语句列表
  ~Block() override = default;

  // 见 Container::append
  // 代码块特殊处理一下，当作 BaseStat 而不是 Container
  virtual auto append(Named *child) -> void;
  virtual auto append(BaseStat *child) -> void;
  virtual auto append(Block *child) -> void;
};

// --------------------------------------------------

class Namespace : public Container {
public:
  Namespace(Container *parent, str name) : Container(parent, std::move(name)) {}
  ~Namespace() override = default;

  // auto mangling(ostream &os, Mangling style) -> void override;
};

// --------------------------------------------------

class Class : public Container {
public:
  ~Class() override = default;

  auto append(Named *child) -> void override;
};

// --------------------------------------------------

class Struct : public Container {
public:
  ~Struct() override = default;

  auto append(Named *child) -> void override;
};

// --------------------------------------------------

class Root : public Namespace {
public:
  Root()           = default;
  ~Root() override = default;

protected:
  auto print_to(ostream &os) const -> void override;
};

class Function : public Named {
protected:
  // auto _mangling() -> str override;
};

} // namespace lumos::ast
