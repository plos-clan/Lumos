#pragma once

#include <type.hpp>

namespace lumos::error {

class AST : public Error {};
class Lexer : public Error {};

} // namespace lumos::error

namespace lumos::compiler {

class Logger {
public:
  enum EType {
    debug,
    info,
    warning,
    error,
    fatal,
    none,
  };

  ostream &os;
  EType    _level = info;

  Logger(ostream &os) : os(os) {}

  auto clear() -> Logger & {
    os << "\ec";
    return *this;
  }

  auto endl() -> Logger & {
    os << std::endl;
    return *this;
  }

  auto color() -> Logger & {
    os << "\e[0m";
    return *this;
  }

  auto fg(u8 r, u8 g, u8 b) -> Logger & {
    os << "\e[38;2;" << r << ';' << g << ';' << b << 'm';
    return *this;
  }

  auto bg(u8 r, u8 g, u8 b) -> Logger & {
    os << "\e[48;2;" << r << ';' << g << ';' << b << 'm';
    return *this;
  }

  auto color(u8 r, u8 g, u8 b) -> Logger & {
    return fg(r, g, b);
  }

  auto level(EType type) -> Logger & {
    _level = type;
    return *this;
  }

  template <typename T>
  auto print(T v) -> Logger & {
    os << v;
    return *this;
  }

  auto operator<<(EType type) -> Logger & {
    if (_level > type) return *this;
    print('[');
    switch (type) {
    case debug: fg(255, 255, 255).print("debug"); break;
    }
    color().print(']');
    return *this;
  }
};

} // namespace lumos::compiler
