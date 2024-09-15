#pragma once

#include <lumos/ast.hpp>
#include <lumos/base.hpp>

namespace lumos {

struct File;
auto loadfile(CTX &ctx, str path) -> File *;

class Preprocesser; // 目前不实现的预处理器

struct CTX {
  HashMap<str, str>    keywords;  // 关键字
  MatchList<void *>    operators; // 运算符
  HashMap<str, File *> files;     // 文件
  ST<AST *>            modules;   // 模块
  ST<ast::Type>        type;

  explicit CTX();

  auto loadfile(str path) -> File * {
    return lumos::loadfile(*this, path);
  }
};

struct ENV {
  Stack<void *> frames;
};

} // namespace lumos
