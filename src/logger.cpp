#include <lumos.hpp>

namespace lumos {

auto Logger::check() -> Logger & {
  if (cnt[Error] > 0 || cnt[Fatal] > 0) {
    endl()
        .print("编译失败：")
        .endl()
        .print("产生了 ")
        .print(cnt[Info])
        .print(" 个提示，")
        .print(cnt[Warn])
        .print(" 个警告，")
        .print(cnt[Error])
        .print(" 个错误，")
        .print(cnt[Fatal])
        .print(" 个致命错误")
        .endl();
    exit(1);
  }
  return *this;
}

auto Logger::clear() -> Logger & {
  os << "\ec";
  return *this;
}

auto Logger::endl() -> Logger & {
  os << std::endl;
  return *this;
}

auto Logger::color() -> Logger & {
  os << "\e[0m";
  return *this;
}

auto Logger::fg(u8 r, u8 g, u8 b) -> Logger & {
  os << "\e[38;2;" << r << ';' << g << ';' << b << 'm';
  return *this;
}

auto Logger::bg(u8 r, u8 g, u8 b) -> Logger & {
  os << "\e[48;2;" << r << ';' << g << ';' << b << 'm';
  return *this;
}

auto Logger::color(u8 r, u8 g, u8 b) -> Logger & {
  return fg(r, g, b);
}

auto Logger::level(EType type) -> Logger & {
  _level = type;
  return *this;
}

auto Logger::print(EType type) -> Logger & {
  if (type >= TypeCnt) return *this;
  cnt[type]++;
  if (_level > type) return *this;
  print('[');
  switch (type) {
  case Debug: fg(128, 192, 255); break;
  case Info: fg(64, 192, 128); break;
  case Warn: fg(255, 192, 0); break;
  case Error: fg(255, 128, 64); break;
  case Fatal: fg(255, 64, 64); break;
  default: throw Fail("未知的日志类型");
  }
  static const char *lut[] = {"Debug", "Info ", "Warn ", "Error", "Fatal"};
  print(lut[type]).color().print("] ");
  return *this;
}

auto Logger::print(const Pos &pos) -> Logger & {
  return print('[')
      .fg(128, 128, 128)
      .print(pos.file)
      .print(':')
      .print(pos.line)
      .print(':')
      .print(pos.col)
      .color()
      .print("] ");
}

auto Logger::debug(strref msg) -> Logger & {
  return print(Debug).print(msg).endl();
}

auto Logger::info(strref msg) -> Logger & {
  return print(Info).print(msg).endl();
}

auto Logger::warn(strref msg) -> Logger & {
  return print(Warn).print(msg).endl();
}

auto Logger::error(strref msg) -> Logger & {
  return print(Error).print(msg).endl();
}

auto Logger::fatal(strref msg) -> Logger & {
  return print(Fatal).print(msg).endl();
}

auto Logger::debug(const Pos &pos, strref msg) -> Logger & {
  return print(Debug).print(pos).print(msg).endl();
}
auto Logger::info(const Pos &pos, strref msg) -> Logger & {
  return print(Info).print(pos).print(msg).endl();
}
auto Logger::warn(const Pos &pos, strref msg) -> Logger & {
  return print(Warn).print(pos).print(msg).endl();
}
auto Logger::error(const Pos &pos, strref msg) -> Logger & {
  return print(Error).print(pos).print(msg).endl();
}
auto Logger::fatal(const Pos &pos, strref msg) -> Logger & {
  return print(Fatal).print(pos).print(msg).endl();
}

auto Logger::fix(strref msg) -> Logger & {
  return print(Info).print("可修复").endl().print(msg).endl();
}

Logger logger(std::cout);

} // namespace lumos
