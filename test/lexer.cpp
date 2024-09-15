#include <lumos.hpp>

using namespace lumos;

auto main() -> int {
  CTX   ctx;
  File *file = ctx.loadfile("../example/helloworld.lm");
  Lexer lexer(ctx, file->path.c_str(), file->data, file->size);
  parser::parse(lexer);

  return 0;
}
