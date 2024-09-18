#pragma once

#include <lumos/base.hpp>

namespace lumos {

__ERRORIMPL__("Lumos");

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

namespace token {
struct TokenPos;
}

class Logger {
  using Pos = token::TokenPos;

public:
  enum EType {
    Debug,
    Info,
    Warn,
    Error,
    Fatal,
    None,
    TypeCnt = None,
  };

  ostream &os;
  EType    _level       = Info;
  size_t   cnt[TypeCnt] = {};

  Logger(ostream &os) : os(os) {}

  auto check() -> Logger &;
  auto clear() -> Logger &;
  auto endl() -> Logger &;
  auto color() -> Logger &;
  auto fg(byte r, byte g, byte b) -> Logger &;
  auto bg(byte r, byte g, byte b) -> Logger &;
  auto color(byte r, byte g, byte b) -> Logger &;
  auto setlevel(EType type) -> Logger &;
  auto level(EType type) -> Logger &;
  auto print(EType type) -> Logger &;
  auto print(const Pos &pos) -> Logger &;

  template <typename T>
  auto print(const T &v) -> Logger & {
    os << v;
    return *this;
  }

  template <typename T>
  auto operator<<(const T &v) -> Logger & {
    return print(v);
  }

  auto debug(const str &msg) -> Logger &;
  auto info(const str &msg) -> Logger &;
  auto warn(const str &msg) -> Logger &;
  auto error(const str &msg) -> Logger &;
  auto fatal(const str &msg) -> Logger &;

  auto debug(const Pos &pos, const str &msg) -> Logger &;
  auto info(const Pos &pos, const str &msg) -> Logger &;
  auto warn(const Pos &pos, const str &msg) -> Logger &;
  auto error(const Pos &pos, const str &msg) -> Logger &;
  auto fatal(const Pos &pos, const str &msg) -> Logger &;

  auto fix(const str &msg) -> Logger &;
};

extern Logger logger;

} // namespace lumos
