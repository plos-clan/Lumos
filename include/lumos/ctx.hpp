#pragma once

#include <type.hpp>

namespace lumos {

struct Ctx {
  str          file;
  HashSet<str> ops;

  explicit Ctx(const str &file) : file(file) {}
};

} // namespace lumos
