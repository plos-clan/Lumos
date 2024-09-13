#pragma once

#include <lumos/base.hpp>

#include "ctx.hpp"

#include <filesystem>

namespace lumos {

struct File {
  str    path;
  void  *data;
  size_t size;
  ~File() {
    free(data);
  }
};

auto loadfile(Ctx &ctx, str path) -> File *;
} // namespace lumos
