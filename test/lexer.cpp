#include <lumos/lexer.hpp>

using namespace lumos::lexer;

constexpr auto example = R"(
fn main {
  println("Hello, world!");
  return 0;
}
)";

auto main() -> int {
  PLexer lexer = new Lexer("<stdin>", example, strlen(example));

  while (lexer->peek() != null) {
    cout << *lexer->get() << endl;
  }

  return 0;
}
