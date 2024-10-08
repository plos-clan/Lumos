#pragma once

#include <lumos/base.hpp>
#include <lumos/ctx.hpp>

namespace lumos {

struct File {
  str    path;
  void  *data;
  size_t size;
  ~File() {
    free(data);
  }
};

} // namespace lumos
