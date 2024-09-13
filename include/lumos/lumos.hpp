#pragma once

#include <lumos/base.hpp>

namespace lumos {

// 是否为空格
static auto isspace(char c) -> bool {
  return c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == '\n';
}
// 是否为数字
static auto isdigit(char c) -> bool {
  return '0' <= c && c <= '9';
}
static auto isdigit(char c, int base) -> bool {
  if (base == 2) return c == '0' || c == '1';
  if (base == 8) return '0' <= c && c <= '7';
  if (base == 10) return '0' <= c && c <= '9';
  if (base == 16) return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
  return false;
}
// 是否为标识符允许的字符
static auto issymc(char c) -> bool {
  if ('A' <= c && c <= 'Z') return true;
  if ('a' <= c && c <= 'z') return true;
  if ('0' <= c && c <= '9') return true;
  if (c == '_' || c == '$') return true;
  if ((byte)c >= 128) return true;
  return false;
}
// 是否为字母
static auto isalpha(char c) -> bool {
  return 'A' <= c && c <= 'Z' || 'a' <= c && c <= 'z';
}
// 是否为字母或数字
static auto isalnum(char c) -> bool {
  return 'A' <= c && c <= 'Z' || 'a' <= c && c <= 'z' || '0' <= c && c <= '9';
}
} // namespace lumos

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
