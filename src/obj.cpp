#pragma once

#include <lumos.hpp>

namespace lumos {

//* ----------------------------------------------------------------------------------------------------
//; LumosObject
//* ----------------------------------------------------------------------------------------------------

u32 LumosObject::next_id = 0;

LumosObject::LumosObject() {
  id = next_id++;
}

auto operator<<(ostream &os, const LumosObject &obj) -> ostream & {
  obj.print_to(os, 0);
  return os;
}

auto LumosObject::check() const -> void {
  throw Error("LumosObject不应被直接使用");
}

auto LumosObject::print_to(ostream &os, i32 indent) const -> void {
  for (i32 i = 0; i < indent * 2; i++) {
    os.put(' ');
  }
  print_to(os);
  os << endl;
  print_children_to(os);
}

auto LumosObject::print_to(ostream &os) const -> void {
  os << "<LumosObject> If you see this node printed, please check the code.";
}

auto LumosObject::print_children_to(ostream &os, i32 indent) const -> void {}

} // namespace lumos
