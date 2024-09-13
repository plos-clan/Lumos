#pragma once

#include <lumos/ast.hpp>
#include <lumos/base.hpp>

namespace lumos {

struct File;

class Preprocesser; // 目前不实现的预处理器

struct Ctx {
  str                  file;      // 当前文件
  HashMap<str, str>    keywords;  // 关键字
  MatchList<void *>    operators; // 运算x符
  HashMap<str, File *> files;     // 文件
  ST<AST *>            modules;   // 模块

  explicit Ctx(const str &file);

  auto loadfile(str path) -> File *;
};

} // namespace lumos
