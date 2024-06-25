#include <lumos/lexer.hpp>

using namespace lumos::lexer;

auto main() -> int {
  PLexer lexer = new Lexer(R"(
fn main {
  println("Hello, world!");
  return 0;
}
)");

  while (lexer->peek().ptr != null) {
    cout << *lexer->get() << endl;
  }

  return 0;
}
