#include <lumos.hpp>

using namespace lumos;

auto main() -> int {
  CTX   ctx;
  File *file = ctx.loadfile("../example/helloworld.lm");
  Lexer lexer(ctx, file->path.c_str(), file->data, file->size);

  for (PToken tok = lexer.get(); tok != null; tok = lexer.get()) {
    cout << tok << endl;
  }

  logger.check();

  // AST *ast = Parser(lexer).parse();

  // if (isinstance<ast::Container>(ast)) {}

  // ast;

  return 0;
}

template <typename T>
struct Tokens {
  T     *arr; // 储存 token 的数组
  size_t len; // 储存当前数组的长度
  size_t cap; // 储存当前数组的容量
  size_t pos; // 储存当前指向的位置

  Tokens(size_t cap = 16);
};

namespace lumos::preprocesser {

void process() {}

} // namespace lumos::preprocesser

namespace lumos::runner {

void run() {}

} // namespace lumos::runner
