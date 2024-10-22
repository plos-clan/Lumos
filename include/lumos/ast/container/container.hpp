#pragma once

#include "../ast.hpp"
#include "../statement.hpp"

namespace lumos::ast {

//* ----------------------------------------------------------------------------------------------------
//; Container
//* ----------------------------------------------------------------------------------------------------

/**
 *\brief 容器（接口）
 * 不强制 Container 有名称
 * 内部元素无法导出的 Container 在 mangling 函数中会抛出错误
 * 例如 lambda 表达式
 *
 */
class Container : public Named {
public:
  // 用于无名节点的构造
  explicit Container(Container *parent);
  // 用于有名节点的构造
  Container(Container *parent, str name);
  ~Container() override = default;

  // 从 Container 中搜索指定名称的节点
  // 该 Container 中搜不到就往上层搜
  virtual auto find(strref name) const -> AST *;

  // child 由调用者分配，所有权转移到类
  virtual auto append(Named *child) -> void;
};

//* ----------------------------------------------------------------------------------------------------
//; 直接继承 Container 的类
//* ----------------------------------------------------------------------------------------------------

/**
 *\brief 代码块（基类）
 *
 */
class Block : public BaseStat, public Container {
public:
  ST<PType>         vars;
  Vector<PBaseStat> code; // 语句列表
  Block()           = default;
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
};

// --------------------------------------------------

class Root : public Namespace {
public:
  Root()           = default;
  ~Root() override = default;

protected:
  auto print_to(ostream &os) const -> void override;
};

class Function {
  struct Param {
    PType type; // 参数类型
    str   name; // 参数名称
    PExpr init; // 默认值
  };

  struct Attribute {
    bool  free;      //
    PExpr freesize;  //
    bool  malloc;    // 是否返回分配的内存
    PExpr allocsize; //
    bool  mtsafe;    //
    bool  mtlock;    // 是否对函数加锁
    bool  nonblock;  //

    void check() const {
      if (mtsafe && mtlock) throw Error("");
    }
  };

  PType         rettype; // 返回值类型
  Vector<Param> params;
  Attribute     attr;

public:
  Function() = default;
};

class NamedFunc : public Named {
protected:
  // auto _mangling() -> str override;
};

} // namespace lumos::ast
