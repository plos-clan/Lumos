#pragma once

#include <lumos/base.hpp>

namespace lumos {

struct File;

template <typename T>
class MatchList {
  Array<Vector<str>, 256> strs;
  Array<Vector<T>, 256>   vals;

public:
  MatchList() = default;

  auto find(const str &s) const -> T * {
    const auto &ss = strs[s[0]];
    const auto &vs = vals[s[0]];
    for (size_t i = 0; i < ss.size(); i++) {
      if (ss[i] == s) return &vs[i];
    }
    return null;
  }

  auto match(cstr s) const -> Pair<const str *, T *> {
    const auto &ss = strs[s[0]];
    const auto &vs = vals[s[0]];
    ssize_t     m = -1, n = 0;
    for (size_t i = 0; i < ss.size(); i++) {
      const auto &t = ss[i];
      if (t.length() > n && strncmp(t.c_str(), s, t.length()) == 0) m = i, n = t.length();
    }
    if (m >= 0) return {&ss[m], (T *)&vs[m]};
    return {null, null};
  }

  void append(const str &s, const T &v) {
    strs[s[0]].push_back(s);
    vals[s[0]].push_back(v);
  }
};

struct Ctx {
  str                  file;
  HashMap<str, str>    keywords; // 关键字
  MatchList<void *>    operators;
  HashMap<str, File *> files;

  explicit Ctx(const str &file);

  auto loadfile(str path) -> File *;
};

} // namespace lumos
