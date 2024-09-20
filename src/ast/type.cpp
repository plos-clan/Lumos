#include <lumos.hpp>

namespace lumos::ast {

void Type::check() const {
  throw Fail("Type::check() 不能被直接调用");
}

void InnerType::check() const {
  throw Fail("InnerType::check() 不能被直接调用");
}

//* ----------------------------------------------------------------------------------------------------
//;
//* ----------------------------------------------------------------------------------------------------

auto Void::tostr() const -> str {
  static const str data = "<void>";
  return data;
}

void Void::check() const {
  // 无
}

} // namespace lumos::ast
