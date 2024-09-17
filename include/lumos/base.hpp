#pragma once

#include <array>
#include <ccomplex>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define $auto const auto

static const auto null = nullptr;

using $char   = const char;
using $short  = const short;
using $ushort = const unsigned short;
using $int    = const int;
using $uint   = const unsigned;
using $long   = const long;
using $ulong  = const unsigned long;
using $float  = const float;
using $double = const double;

using int128_t   = __int128;
using uint128_t  = unsigned __int128;
using float16_t  = _Float16;
using float32_t  = float;
using float64_t  = double;
using float128_t = __float128;

using i8   = int8_t;
using u8   = uint8_t;
using i16  = int16_t;
using u16  = uint16_t;
using i32  = int32_t;
using u32  = uint32_t;
using i64  = int64_t;
using u64  = uint64_t;
using i128 = int128_t;
using u128 = uint128_t;
using f16  = float16_t;
using f32  = float32_t;
using f64  = float64_t;
using f128 = float128_t;

using $i8   = const int8_t;
using $u8   = const uint8_t;
using $i16  = const int16_t;
using $u16  = const uint16_t;
using $i32  = const int32_t;
using $u32  = const uint32_t;
using $i64  = const int64_t;
using $u64  = const uint64_t;
using $i128 = const int128_t;
using $u128 = const uint128_t;
using $f16  = const float16_t;
using $f32  = const float32_t;
using $f64  = const float64_t;
using $f128 = const float128_t;

using cfloat  = std::complex<float>;
using cdouble = std::complex<double>;

using cint8_t     = std::complex<int8_t>;
using cuint8_t    = std::complex<uint8_t>;
using cint16_t    = std::complex<int16_t>;
using cuint16_t   = std::complex<uint16_t>;
using cint32_t    = std::complex<int32_t>;
using cuint32_t   = std::complex<uint32_t>;
using cint64_t    = std::complex<int64_t>;
using cuint64_t   = std::complex<uint64_t>;
using cint128_t   = std::complex<int128_t>;
using cuint128_t  = std::complex<uint128_t>;
using cfloat16_t  = std::complex<float16_t>;
using cfloat32_t  = std::complex<float32_t>;
using cfloat64_t  = std::complex<float64_t>;
using cfloat128_t = std::complex<float128_t>;

namespace lumos {

// template <typename T>
// class BaseStringBuilder {
//   u32 len = 0;
//   u32 cap = 0;
//   T  *ptr = null;

// public:
//   BaseStringBuilder() = default;
//   BaseStringBuilder(size_t cap) : cap(cap) {
//     if (cap == 0) return;
//     ptr = malloc(cap * sizeof(T));
//   }
//   ~BaseStringBuilder() {
//     if (ptr == null) return;
//     free(ptr);
//   }

//   auto empty() const -> bool {
//     return len == 0;
//   }

//   auto length() const -> size_t {
//     return len;
//   }

//   auto size() const -> size_t {
//     return len;
//   }

//   auto substr(size_t p) const -> BaseStringBuilder {
//     if (p >= len) return {};
//     return {ptr + p, len - p};
//   }

//   auto operator[](uint32_t i) const -> const T & {
//     return ptr[i];
//   }

//   auto operator+=(const T *s) -> BaseStringBuilder & {
//     if (s == null) return *this;
//     size_t n = strlen(s);
//     if (n == 0) return *this;
//     if (len + n + 1 > cap) {
//       cap = len + n + 1;
//       ptr = realloc(ptr, cap * sizeof(T));
//     }
//     memcpy(ptr + len, s, n * sizeof(T));
//     len      += n;
//     ptr[len]  = 0;
//     return *this;
//   }

//   auto operator+=(const BaseStringBuilder &rhs) -> BaseStringBuilder & {
//     if (rhs.len == 0) return *this;
//     if (len + rhs.len + 1 > cap) {
//       cap = len + rhs.len + 1;
//       ptr = realloc(ptr, cap * sizeof(T));
//     }
//     memcpy(ptr + len, rhs.ptr, rhs.len * sizeof(T));
//     len      += rhs.len;
//     ptr[len]  = 0;
//     return *this;
//   }

//   auto operator==(const BaseStringBuilder &rhs) const -> bool {
//     if (len != rhs.len) return false;
//     for (uint32_t i = 0; i < size; i++) {
//       if (ptr[i] != rhs.ptr[i]) return false;
//     }
//     return true;
//   }
// };

template <typename T>
class BaseString {
  struct Data {
    u32 _rc;     // 引用计数
    u32 _len;    // 数据长度
    T   _data[]; // 数据 (动态分配)
  };

private:
  u32 len = 0;    // 子字符串长度
  u32 pos = 0;    // 子字符串起始位置
  T  *ptr = null; // 原始字符串数据

  static constexpr T nullchar = 0;

public:
  static auto strlen(const T *s) -> size_t {
    size_t len = 0;
    for (; s[len]; len++) {}
    return len;
  }

  class Iterator {
  public:
    const T *ptr = null;

    Iterator() = default;
    Iterator(const T *p) : ptr(p) {}
    Iterator(const Iterator &it) : ptr(it.ptr) {}

    auto operator==(const Iterator &it) const -> bool {
      return ptr == it.ptr;
    }

    auto operator!=(const Iterator &it) const -> bool {
      return ptr != it.ptr;
    }

    auto operator*() const -> T {
      return *ptr;
    }
  };

  class ForwardIterator : public Iterator {
    using Iterator::ptr;

  public:
    auto base() const -> ForwardIterator {
      return *this;
    }

    auto operator+(size_t i) const -> ForwardIterator {
      return {ptr + i};
    }

    auto operator++() -> ForwardIterator & {
      ptr++;
      return *this;
    }

    auto operator++(int) -> ForwardIterator & {
      auto t = *this;
      ptr++;
      return t;
    }

    auto operator-(size_t i) const -> ForwardIterator {
      return {ptr - i};
    }

    auto operator--() -> ForwardIterator & {
      ptr--;
      return *this;
    }

    auto operator--(int) -> ForwardIterator & {
      auto t = *this;
      ptr--;
      return t;
    }

    auto operator[](size_t i) const -> T {
      return ptr[i];
    }
  };

  class BackwardIterator : public Iterator {
    using Iterator::ptr;

  public:
    auto base() const -> BackwardIterator {
      return *this;
    }

    auto operator+(size_t i) const -> BackwardIterator {
      return {ptr - i};
    }

    auto operator++() -> BackwardIterator & {
      ptr--;
      return *this;
    }

    auto operator++(int) -> BackwardIterator & {
      auto t = *this;
      ptr--;
      return t;
    }

    auto operator-(size_t i) const -> BackwardIterator {
      return {ptr + i};
    }

    auto operator--() -> BackwardIterator & {
      ptr++;
      return *this;
    }

    auto operator--(int) -> BackwardIterator & {
      auto t = *this;
      ptr++;
      return t;
    }

    auto operator[](size_t i) const -> T {
      return ptr[-(ssize_t)i];
    }
  };

  auto begin() const -> ForwardIterator {
    return {ptr};
  }

  auto end() const -> ForwardIterator {
    return {ptr + len};
  }

  auto rbegin() const -> BackwardIterator {
    return {ptr + len - 1};
  }

  auto rend() const -> BackwardIterator {
    return {ptr - 1};
  }

#define dataptr ((Data *)((size_t)this->ptr - this->pos - sizeof(Data)))
#define rc      (dataptr->_rc)
#define rawlen  (dataptr->_len)
#define data    (dataptr->_data)

private:
  void allocdata() {
    void *addr = malloc(sizeof(Data) + (len + 1) * sizeof(T));
    ptr        = (T *)((size_t)addr + sizeof(Data));
    rc         = 1;
  }

  void allocdata(size_t n) {
    len = n, pos = 0, ptr = null;
    if (len > 0) allocdata();
  }

  void freedata() {
    if (ptr == null) return;
    void *addr = (void *)((size_t)ptr - pos - sizeof(Data));
    if (--rc == 0) free(addr);
  }

  void setdata(const T *s) {
    len = 0, pos = 0, ptr = null;
    if (s == null) return;
    if (len = strlen(s); len == 0) return;
    allocdata();
    memcpy(data, s, (len + 1) * sizeof(T));
  }

  void setdata(const T *s, size_t n) {
    len = n, pos = 0, ptr = null;
    if (len == 0) return;
    allocdata();
    memcpy(data, s, len * sizeof(T));
    data[n] = 0;
  }

  void resetdata(const T *s) {
    freedata();
    setdata(s);
  }

  void resetdata(const T *s, size_t n) {
    freedata();
    setdata(s, n);
  }

public:
  BaseString() = default;
  // 预分配一个长度为 len 的字符串 (不初始化)
  BaseString(size_t n) {
    allocdata(n);
  }
  BaseString(const T *s) {
    setdata(s);
  }
  BaseString(const T *s, size_t n) {
    setdata(s, n);
  }
  BaseString(const std::string &s) {
    setdata(s.c_str(), s.length());
  }
  BaseString(u32 len, u32 pos, T *ptr) : len(len), pos(pos), ptr(ptr) {
    if (ptr != null) rc++;
  }
  BaseString(const T *begin, const T *end) : len(end - begin) {
    if (len == 0) return;
    allocdata(len);
    for (size_t i = 0; i < len; i++) {
      ptr[i] = begin[i];
    }
  }
  BaseString(const Iterator &begin, const Iterator &end) : len(end.ptr - begin.ptr) {
    if (len == 0) return;
    allocdata(len);
    for (size_t i = 0; i < len; i++) {
      ptr[i] = begin.ptr[i];
    }
  }
  BaseString(const BaseString &s) : len(s.len), pos(s.pos), ptr(s.ptr) {
    if (ptr != null) rc++;
  }
  BaseString(BaseString &&s) : len(s.len), pos(s.pos), ptr(s.ptr) {
    s.len = 0, s.pos = 0, s.ptr = null;
  }
  ~BaseString() {
    freedata();
  }

  auto operator=(const BaseString &s) -> BaseString & {
    if (this == &s || ptr == s.ptr) return *this;
    freedata();
    len = s.len, pos = s.pos, ptr = s.ptr;
    if (ptr != null) rc++;
    return *this;
  }

  auto operator=(BaseString &&s) -> BaseString & {
    if (this == &s) return *this;
    freedata();
    len = s.len, pos = s.pos, ptr = s.ptr;
    s.len = 0, s.pos = 0, s.ptr = null;
    return *this;
  }

  auto operator=(const std::string &s) -> BaseString & {
    resetdata(s.c_str(), s.length());
    return *this;
  }

  auto operator=(const T *s) -> BaseString & {
    resetdata(s);
    return *this;
  }

  auto substr(size_t p) const -> BaseString {
    if (pos >= len) return {};
    return {(u32)(len - p), (u32)(pos + p), ptr + p};
  }

  auto substr(size_t p, size_t n) const -> BaseString {
    if (p >= len) return {};
    if (p + n > len) n = len - p;
    return {(u32)n, (u32)(pos + p), ptr + p};
  }

  auto c_str() -> const T * {
    if (len == 0) return &nullchar;
    if (pos == 0 && len == rawlen) return ptr;
    auto datap = dataptr;
    auto _ptr  = ptr;
    allocdata(len);
    memcpy(data, _ptr, len * sizeof(T));
    data[len] = 0;
    if (--datap->_rc == 0) free(datap);
    return ptr;
  }

  // 获取原始字符串数据，预分配后这样写入数据
  auto getptr() -> T * {
    return ptr;
  }

  auto empty() const -> bool {
    return len == 0;
  }

  operator bool() const {
    return len != 0;
  }

  auto operator!() const -> bool {
    return len == 0;
  }

  auto length() const -> size_t {
    return len;
  }

  auto size() const -> size_t {
    return len * sizeof(T);
  }

  auto operator[](size_t i) const -> T {
    return ptr[i];
  }

  auto equals(const BaseString &s) const -> bool {
    if (len != s.len) return false;
    if (ptr == s.ptr) return true;
    for (size_t i = 0; i < len; i++) {
      if (ptr[i] != s.ptr[i]) return false;
    }
    return true;
  }

  auto equals(const std::string &s) const -> bool {
    if (len != s.length()) return false;
    for (size_t i = 0; i < len; i++) {
      if (ptr[i] != s[i]) return false;
    }
    return true;
  }

  auto equals(const T *s) const -> bool {
    if (s == null) return false;
    for (size_t i = 0; i < len; i++) {
      if (ptr[i] != s[i]) return false;
    }
    return s[len] == 0;
  }

  auto equals(const T *s, size_t n) const -> bool {
    if (s == null) return false;
    if (len != n) return false;
    for (size_t i = 0; i < len; i++) {
      if (ptr[i] != s[i]) return false;
    }
    return true;
  }

  auto operator==(const BaseString &s) const -> bool {
    return equals(s);
  }

  auto operator==(const std::string &s) const -> bool {
    return equals(s);
  }

  auto operator==(const T *s) const -> bool {
    return equals(s);
  }

  auto operator!=(const BaseString &s) const -> bool {
    return !equals(s);
  }

  auto operator!=(const std::string &s) const -> bool {
    return !equals(s);
  }

  auto operator!=(const T *s) const -> bool {
    return !equals(s);
  }

  auto cmp(const BaseString &s) const -> int {
    size_t n = len < s.len ? len : s.len;
    for (size_t i = 0; i < n; i++) {
      if (ptr[i] != s.ptr[i]) return ptr[i] - s.ptr[i];
    }
    return len - s.len;
  }

  auto cmp(const std::string &s) const -> int {
    size_t n = len < s.length() ? len : s.length();
    for (size_t i = 0; i < n; i++) {
      if (ptr[i] != s[i]) return ptr[i] - s[i];
    }
    return len - s.length();
  }

  auto cmp(const T *s) const -> int {
    if (s == null) return 1;
    for (size_t i = 0; i < len; i++) {
      if (ptr[i] != s[i]) return ptr[i] - s[i];
    }
    return s[len];
  }

  auto cmp(const T *s, size_t n) const -> int {
    if (s == null) return 1;
    size_t m = len < n ? len : n;
    for (size_t i = 0; i < m; i++) {
      if (ptr[i] != s[i]) return ptr[i] - s[i];
    }
    return len - n;
  }

  auto ncmp(const BaseString &s, size_t n) const -> int {
    size_t m = len < s.len ? len : s.len;
    size_t l = m < n ? m : n;
    for (size_t i = 0; i < l; i++) {
      if (ptr[i] != s.ptr[i]) return ptr[i] - s.ptr[i];
    }
    return m < n ? len - s.len : 0;
  }

  auto ncmp(const std::string &s, size_t n) const -> int {
    size_t m = len < s.length() ? len : s.length();
    size_t l = m < n ? m : n;
    for (size_t i = 0; i < l; i++) {
      if (ptr[i] != s[i]) return ptr[i] - s[i];
    }
    return m < n ? len - s.length() : 0;
  }

  auto ncmp(const T *s, size_t n) const -> int {
    if (s == null) return 1;
    size_t m = len < n ? len : n;
    for (size_t i = 0; i < m; i++) {
      if (ptr[i] != s[i]) return ptr[i] - s[i];
    }
    return len < n ? -1 : 0;
  }

  auto operator<(const BaseString &s) const -> bool {
    return cmp(s) < 0;
  }

  auto operator<(const std::string &s) const -> bool {
    return cmp(s) < 0;
  }

  auto operator<(const T *s) const -> bool {
    return cmp(s) < 0;
  }

  auto operator<=(const BaseString &s) const -> bool {
    return cmp(s) <= 0;
  }

  auto operator<=(const std::string &s) const -> bool {
    return cmp(s) <= 0;
  }

  auto operator<=(const T *s) const -> bool {
    return cmp(s) <= 0;
  }

  auto operator>(const BaseString &s) const -> bool {
    return cmp(s) > 0;
  }

  auto operator>(const std::string &s) const -> bool {
    return cmp(s) > 0;
  }

  auto operator>(const T *s) const -> bool {
    return cmp(s) > 0;
  }

  auto operator>=(const BaseString &s) const -> bool {
    return cmp(s) >= 0;
  }

  auto operator>=(const std::string &s) const -> bool {
    return cmp(s) >= 0;
  }

  auto operator>=(const T *s) const -> bool {
    return cmp(s) >= 0;
  }

  auto at(size_t i) const -> T {
    if (i >= len) return 0;
    return ptr[i];
  }

  auto at(ssize_t i) const -> T {
    if (i < 0) i += len;
    if (i < 0 || i >= len) return 0;
    return ptr[i];
  }

  template <typename U = size_t>
  auto hash() const -> U {
    U h = 0;
    for (U i = 0; i < len; i++) {
      h = h * 131 + ptr[i];
    }
    return h;
  }

  auto hash16() const -> uint16_t {
    uint16_t h = 0;
    for (size_t i = 0; i < len; i++) {
      h = h * 131 + ptr[i];
    }
    return h;
  }

  auto hash32() const -> uint32_t {
    uint32_t h = 0;
    for (size_t i = 0; i < len; i++) {
      h = h * 131 + ptr[i];
    }
    return h;
  }

  auto hash64() const -> uint64_t {
    uint64_t h = 0;
    for (size_t i = 0; i < len; i++) {
      h = h * 131 + ptr[i];
    }
    return h;
  }

  friend auto operator<<(std::ostream &os, const BaseString &s) -> std::ostream & {
    for (size_t i = 0; i < s.len; i++) {
      os << s.ptr[i];
    }
    return os;
  }

  auto append(const BaseString &s) const -> BaseString {
    BaseString t(len + s.len);
    memcpy(t.ptr, ptr, len * sizeof(T));
    memcpy(t.ptr + len, s.ptr, s.len * sizeof(T));
    return t;
  }

  auto append(const std::string &s) const -> BaseString {
    BaseString t(len + s.length());
    memcpy(t.ptr, ptr, len * sizeof(T));
    memcpy(t.ptr + len, s.c_str(), s.length() * sizeof(T));
    return t;
  }

  auto append(const T *s) const -> BaseString {
    if (s == null) return *this;
    size_t     n = strlen(s);
    BaseString t(len + n);
    memcpy(t.ptr, ptr, len * sizeof(T));
    memcpy(t.ptr + len, s, n * sizeof(T));
    return t;
  }

  auto append(const T *s, size_t n) const -> BaseString {
    if (s == null) return *this;
    BaseString t(len + n);
    memcpy(t.ptr, ptr, len * sizeof(T));
    memcpy(t.ptr + len, s, n * sizeof(T));
    return t;
  }

  auto append(T c) const -> BaseString {
    BaseString t(len + 1);
    memcpy(t.ptr, ptr, len * sizeof(T));
    t.ptr[len] = c;
    return t;
  }

  auto operator+(const BaseString &s) const -> BaseString {
    append(s);
  }

  auto operator+(const std::string &s) const -> BaseString {
    append(s);
  }

  auto operator+(const T *s) const -> BaseString {
    append(s);
  }

  auto operator+(T c) const -> BaseString {
    append(c);
  }

  auto prepend(const BaseString &s) const -> BaseString {
    BaseString t(len + s.len);
    memcpy(t.ptr, s.ptr, s.len * sizeof(T));
    memcpy(t.ptr + s.len, ptr, len * sizeof(T));
    return t;
  }

  auto prepend(const std::string &s) const -> BaseString {
    BaseString t(s.length() + len);
    memcpy(t.ptr, s.c_str(), s.length() * sizeof(T));
    memcpy(t.ptr + s.length(), ptr, len * sizeof(T));
    return t;
  }

  auto prepend(const T *s) const -> BaseString {
    if (s == null) return *this;
    size_t     n = strlen(s);
    BaseString t(n + len);
    memcpy(t.ptr, s, n * sizeof(T));
    memcpy(t.ptr + n, ptr, len * sizeof(T));
    return t;
  }

  auto prepend(const T *s, size_t n) const -> BaseString {
    if (s == null) return *this;
    BaseString t(n + len);
    memcpy(t.ptr, s, n * sizeof(T));
    memcpy(t.ptr + n, ptr, len * sizeof(T));
    return t;
  }

  auto prepend(T c) const -> BaseString {
    BaseString t(len + 1);
    t.ptr[0] = c;
    memcpy(t.ptr + 1, ptr, len * sizeof(T));
    return t;
  }

  friend auto operator+(std::string &s, const BaseString &t) -> BaseString {
    t.prepend(s);
  }

  friend auto operator+(const T *s, const BaseString &t) -> BaseString {
    t.prepend(s);
  }

  friend auto operator+(T c, const BaseString &t) -> BaseString {
    t.prepend(c);
  }

  auto remove(const T &c) const -> BaseString {
    size_t n = 0;
    for (size_t i = 0; i < len; i++) {
      if (ptr[i] != c) n++;
    }
    BaseString t(n);
    for (size_t i = 0, j = 0; i < len; i++) {
      if (ptr[i] != c) t.ptr[j++] = ptr[i];
    }
    return t;
  }

  auto operator*(size_t n) const -> BaseString {
    BaseString t(len * n);
    for (size_t i = 0; i < n; i++) {
      memcpy(t.ptr + i * len, ptr, len * sizeof(T));
    }
    return t;
  }

  operator std::string() const {
    return std::string(ptr, len);
  }

#undef rc
#undef data
};

} // namespace lumos

namespace std {

template <>
template <typename T>
struct hash<lumos::BaseString<T>> {
  auto operator()(const lumos::BaseString<T> &s) const -> size_t {
    return s.hash();
  }
};

template struct hash<lumos::BaseString<char>>;
template struct hash<lumos::BaseString<wchar_t>>;
template struct hash<lumos::BaseString<char16_t>>;
template struct hash<lumos::BaseString<char32_t>>;

} // namespace std

using str = lumos::BaseString<char>;

using sstream  = std::stringstream;
using isstream = std::istringstream;
using osstream = std::ostringstream;
using istream  = std::istream;
using ostream  = std::ostream;
using iostream = std::iostream;
using ifstream = std::ifstream;
using ofstream = std::ofstream;
using fstream  = std::fstream;

using $str      = const std::string;
using $sstream  = const std::stringstream;
using $isstream = const std::istringstream;
using $osstream = const std::ostringstream;
using $istream  = const std::istream;
using $ostream  = const std::ostream;
using $iostream = const std::iostream;
using $ifstream = const std::ifstream;
using $ofstream = const std::ofstream;
using $fstream  = const std::fstream;

using byte  = u8;
using bytes = std::vector<u8>;

template <typename T>
using Vector = std::vector<T>;
template <typename T>
using List = std::list<T>;
template <typename T, size_t N>
using Array = std::array<T, N>;
template <typename K, typename V>
using Map = std::map<K, V>;
template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;
template <typename T>
using Set = std::set<T>;
template <typename T>
using HashSet = std::unordered_set<T>;
template <typename T1, typename T2>
using Pair = std::pair<T1, T2>;
template <typename T>
using init_list = std::initializer_list<T>;
template <typename T>
using Stack = std::stack<T>;
template <typename... Args>
using Tuple = std::tuple<Args...>;

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

using Error = std::runtime_error;

using Regex = std::regex;

template <typename T>
using ST = HashMap<str, T>;

inline static auto println(const str &s) {
  cout << s << endl;
}

using cstr = const char *;

template <typename T>
using Ptr = std::shared_ptr<T>;

struct _refcount_ {
  size_t _rc_ = 0;
};

template <typename T>
class _P_ {
#define rc (((_refcount_ *)ptr)->_rc_)

public:
  T *ptr = null;

  _P_() = default;

  _P_(T *p) : ptr(p) {
    if (ptr) rc++;
  }

  template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<T, U>>>
  _P_(U *p) : ptr(static_cast<T *>(p)) {
    if (ptr) rc++;
  }

  _P_(const _P_<T> &p) : ptr(p.ptr) {
    if (ptr) rc++;
  }

  template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<T, U>>>
  _P_(const _P_<U> &p) : ptr(static_cast<T *>(p.ptr)) {
    if (ptr) rc++;
  }

  _P_(_P_<T> &&p) noexcept : ptr(p.ptr) {
    p.ptr = null;
  }

  template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<T, U>>>
  _P_(_P_<U> &&p) noexcept : ptr(p.ptr) {
    p.ptr = null;
  }

  auto operator=(const _P_<T> &p) -> _P_<T> & {
    if (this == &p) return *this;
    if (ptr != null && --rc == 0) delete ptr;
    ptr = p.ptr;
    rc++;
    return *this;
  }

  template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<T, U>>>
  auto operator=(const _P_<U> &p) -> _P_<T> & {
    if (this == &p) return *this;
    if (ptr != null && --rc == 0) delete ptr;
    ptr = static_cast<T *>(p.ptr);
    rc++;
    return *this;
  }

  auto operator=(_P_<T> &&p) noexcept -> _P_<T> & {
    if (this == &p) return *this;
    if (ptr != null && --rc == 0) delete ptr;
    ptr   = p.ptr;
    p.ptr = null;
    return *this;
  }

  template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<T, U>>>
  auto operator=(_P_<U> &&p) noexcept -> _P_<T> & {
    if (this == &p) return *this;
    if (ptr != null && --rc == 0) delete ptr;
    ptr   = static_cast<T *>(p.ptr);
    p.ptr = null;
    return *this;
  }

  ~_P_() {
    if (ptr != null && --rc == 0) delete ptr;
  }

  template <typename U>
  auto operator==(const _P_<U> &p) const -> bool {
    return ptr == p.ptr;
  }

  template <typename U>
  auto operator==(const U *p) const -> bool {
    return ptr == p;
  }

  auto operator==(const std::nullptr_t &p) const -> bool {
    return ptr == p;
  }

  auto operator*() const -> T & {
    return *ptr;
  }

  auto operator->() const -> T * {
    return ptr;
  }

  friend auto operator<<(ostream &u, const _P_<T> &p) -> ostream & {
    return u << p.ptr;
  }

  template <typename U>
  friend auto operator<<(ostream &u, _P_<T> &p) -> ostream & {
    return u << p.ptr;
  }

  friend auto operator>>(istream &u, const _P_<T> &p) -> istream & {
    return u >> p.ptr;
  }

  friend auto operator>>(istream &u, _P_<T> &p) -> istream & {
    return u >> p.ptr;
  }

  operator T *() const {
    return ptr;
  }

  template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<U, T>>>
  operator U *() const {
    return static_cast<U *>(ptr);
  }

#undef rc
};

#define Pstruct(name)                                                                              \
  struct name;                                                                                     \
  using P##name = _P_<name>
#define Pclass(name)                                                                               \
  class name;                                                                                      \
  using P##name = _P_<name>
#define pstruct(name)                                                                              \
  struct name;                                                                                     \
  using P##name = _P_<name>;                                                                       \
  struct name : private _refcount_
#define pclass(name)                                                                               \
  class name;                                                                                      \
  using P##name = _P_<name>;                                                                       \
  class name : private _refcount_
#define pusing(space, name)                                                                        \
  using space::name;                                                                               \
  using space::P##name;

template <typename T1, typename T2>
static inline auto isinstance(const T2 *ptr) -> bool {
  static_assert(std::is_base_of_v<T2, T1>);
  return dynamic_cast<const T1 *>(ptr) != null;
}

#include <gmpxx.h>

class mpz : public mpz_class {
public:
  using mpz_class::mpz_class;
  mpz(str s, int base = 10) : mpz_class(s.c_str(), base) {}
};

class mpq : public mpq_class {
public:
  using mpq_class::mpq_class;
  mpq(str s, int base = 10) : mpq_class(s.c_str(), base) {}
};

class mpf : public mpf_class {
public:
  using mpf_class::mpf_class;
  mpf(str s, int base = 10) : mpf_class(s.c_str(), base) {}
};

namespace lumos {

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
      if (t.length() > n && t.ncmp(s, t.length()) == 0) m = i, n = t.length();
    }
    if (m >= 0) return {&ss[m], (T *)&vs[m]};
    return {null, null};
  }

  void append(const str &s, const T &v) {
    strs[s[0]].push_back(s);
    vals[s[0]].push_back(v);
  }
};

struct CTX;

struct ENV;

} // namespace lumos

#define __ERRORIMPL__(name)                                                                        \
  /* 用户输入错误 */                                                                         \
  class Error : public ::Error {                                                                   \
  public:                                                                                          \
    explicit Error(const str &msg) : ::Error(name ": " + msg) {}                                   \
  };                                                                                               \
  /* 程序内部错误 */                                                                         \
  class Fail : public ::Error {                                                                    \
  public:                                                                                          \
    explicit Fail(const str &msg) : ::Error("[fail] " name ": " + msg) {}                          \
  };
