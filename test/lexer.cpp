#include <lumos.hpp>

using namespace lumos;

constexpr auto example = R"(
using "std" -> std;

fn main([int argc, char** argv, [char** envp]]) -> int;

@assert(argc >= 1)
fn main(int argc, char* argv[argc]) -> int {
  std.println("Argc is 1.") if argc == 1;
  for (var a from 1 to argc) {
    std.println("Hello, world!");
  }
  std.println(123., .123, 123.456);
  return 0;
}
)";

auto main() -> int {
  Ctx   ctx("<stdin>");
  Lexer lexer(ctx, example, strlen(example));
  parser::parse(lexer);

  while (lexer.peek() != null) {
    cout << lexer.get() << endl;
  }

  return 0;
}
