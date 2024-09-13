#pragma once

#include <type.hpp>

namespace lumos {

//* ----------------------------------------------------------------------------------------------------
//; LumosObject
//* ----------------------------------------------------------------------------------------------------

class LumosObject : private RefCount {
public:
  u32        id;      // 对象的 ID
  static u32 next_id; // 用来给每个对象分配 ID

  LumosObject();
  virtual ~LumosObject() = default;

  // 输出 直接调用 print_to 不应该修改
  friend auto operator<<(ostream &os, const LumosObject &obj) -> ostream &;

  // debug 用
  // 检查对象是否有错误
  // 有错误直接抛出异常
  virtual void check() const;

protected:
  virtual auto print_to(ostream &os) const -> void;
  virtual auto print_children_to(ostream &os, i32 indent = 0) const -> void;

private:
  auto print_to(ostream &os, i32 indent) const -> void;
};

} // namespace lumos
