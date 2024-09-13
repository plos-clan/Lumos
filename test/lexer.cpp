#include <lumos.hpp>

using namespace lumos::lexer;

// constexpr auto example = R"(
// using "std" -> std;

// fn main([int argc, char** argv, [char** envp]]) -> int;

// @assert(argc >= 1)
// fn main(int argc, char* argv[argc]) -> int {
//   std.println("Argc is 1.") if argc == 1;
//   for (var a from 1 to argc) {
//     std.println("Hello, world!");
//   }
//   std.println(123., .123, 123.456);
//   return 0;
// }
// )";

constexpr auto example = R"(
1
123
0x123
123z
123q
123.456q
123.456e+1
0x1e+1
)";

auto main() -> int {
  lumos::Ctx ctx("<stdin>");
  Lexer      lexer(ctx, example, strlen(example));

  while (lexer.peek() != null) {
    cout << lexer.get() << endl;
  }

  return 0;
}
