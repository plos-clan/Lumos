#pragma once

#include <lumos/base.hpp>

namespace lumos {
enum class Mangling : u8 {
  none,        // 不导出
  lumos,       // lumos 风格
  lumos_full,  // lumos 风格 (完整的信息)
  c,           // c 风格
  cpp,         // c++ 风格
  def = lumos, // 默认风格 (lumos)
};
} // namespace lumos
