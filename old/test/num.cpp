#include <lumos.hpp>

using namespace lumos;

auto main() -> int {

  std::string input;
  while (true) {
    std::cout << "输入：" << std::flush;
    std::getline(std::cin, input);
    if (input.empty()) break;
    try {
      token::num_data(input).println();
    } catch (const std::exception &e) {
      std::cerr << "输入错误：" << std::endl;
      std::cerr << e.what() << std::endl;
    }
  }

  return 0;
}
