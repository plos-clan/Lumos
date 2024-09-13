#pragma once

#include "../../mangling.hpp"
#include "../../obj.hpp"
#include "decl.hpp"

namespace lumos::ast {

//* ----------------------------------------------------------------------------------------------------
//; AST Named
//* ----------------------------------------------------------------------------------------------------

class AST : public LumosObject {
public:
  AST()           = default;
  ~AST() override = default;
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
