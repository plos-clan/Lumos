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

#include <gmpxx.h>

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

namespace lumos::base {

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

  class LineIterator {
  private:
    void next() {
      beg = end;
      for (; end < raw.len && raw.ptr[end] != '\n'; end++) {}
    }

  public:
    const BaseString &raw;
    size_t            beg = 0;
    size_t            end = 0;

    LineIterator(const BaseString &raw) : raw(raw) {
      next();
    }
    LineIterator(const BaseString &raw, size_t len) : raw(raw), end(len) {}

    auto operator==(const LineIterator &it) const -> bool {
      return end == it.end;
    }

    auto operator!=(const LineIterator &it) const -> bool {
      return end != it.end;
    }

    auto operator*() const -> BaseString {
      return raw.substr(beg, end - beg);
    }

    auto operator++() -> LineIterator & {
      end++;
      next();
      return *this;
    }

    auto operator++(int) -> LineIterator & {
      auto t = *this;
      end++;
      next();
      return t;
    }
  };

  class LineIteratorWrapper {
  private:
    const BaseString &raw;

  public:
    LineIteratorWrapper(const BaseString &raw) : raw(raw) {}

    auto begin() const -> LineIterator {
      return {raw};
    }

    auto end() const -> LineIterator {
      return {raw, raw.len};
    }
  };

  auto lines() const -> LineIteratorWrapper {
    return {*this};
  }

#define dataptr ((Data *)((size_t)this->ptr - this->pos - sizeof(Data)))
#define rc      (dataptr->_rc)
#define rawlen  (dataptr->_len)
#define data    (dataptr->_data)

private:
  void allocdata() {
    void *addr = malloc(sizeof(Data) + (len + 1) * sizeof(T));
    ptr        = (T *)((size_t)addr + sizeof(Data));
    rawlen     = len;
    rc         = 1;
  }

  void allocdata(size_t n) {
    len = n, pos = 0, ptr = null;
    if (len > 0) allocdata();
  }

  void freedata() {
    if (ptr == null) return;
    if (--rc == 0) free(dataptr);
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
    if (p >= len) return {};
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

  auto getptr() const -> const T * {
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

  auto at(ssize_t i) const -> T {
    if (i < 0) i += len;
    if (i < 0 || i >= len) return 0;
    return ptr[i];
  }

  auto find(const BaseString &s) const -> ssize_t {
    if (s.len == 0) return 0;
    if (s.len > len) return -1;
    for (size_t i = 0; i <= len - s.len; i++) {
      if (memcmp(ptr + i, s.ptr, s.len * sizeof(T)) == 0) return i;
    }
    return -1;
  }

  auto find(const std::string &s) const -> ssize_t {
    if (s.length() == 0) return 0;
    if (s.length() > len) return -1;
    for (size_t i = 0; i <= len - s.length(); i++) {
      if (memcmp(ptr + i, s.c_str(), s.length() * sizeof(T)) == 0) return i;
    }
    return -1;
  }

  auto find(const T *s) const -> ssize_t {
    if (s == null) return -1;
    size_t n = strlen(s);
    if (n == 0) return 0;
    if (n > len) return -1;
    for (size_t i = 0; i <= len - n; i++) {
      if (memcmp(ptr + i, s, n * sizeof(T)) == 0) return i;
    }
    return -1;
  }

  auto find(const T *s, size_t n) const -> ssize_t {
    if (s == null) return -1;
    if (n == 0) return 0;
    if (n > len) return -1;
    for (size_t i = 0; i <= len - n; i++) {
      if (memcmp(ptr + i, s, n * sizeof(T)) == 0) return i;
    }
    return -1;
  }

  auto find(T c) const -> ssize_t {
    for (size_t i = 0; i < len; i++) {
      if (ptr[i] == c) return i;
    }
    return -1;
  }

  auto rfind(const BaseString &s) const -> ssize_t {
    if (s.len == 0) return len;
    if (s.len > len) return -1;
    for (ssize_t i = len - s.len; i >= 0; i--) {
      if (memcmp(ptr + i, s.ptr, s.len * sizeof(T)) == 0) return i;
    }
    return -1;
  }

  auto rfind(const std::string &s) const -> ssize_t {
    if (s.length() == 0) return len;
    if (s.length() > len) return -1;
    for (ssize_t i = len - s.length(); i >= 0; i--) {
      if (memcmp(ptr + i, s.c_str(), s.length() * sizeof(T)) == 0) return i;
    }
    return -1;
  }

  auto rfind(const T *s) const -> ssize_t {
    if (s == null) return -1;
    size_t n = strlen(s);
    if (n == 0) return len;
    if (n > len) return -1;
    for (ssize_t i = len - n; i >= 0; i--) {
      if (memcmp(ptr + i, s, n * sizeof(T)) == 0) return i;
    }
    return -1;
  }

  auto rfind(const T *s, size_t n) const -> ssize_t {
    if (s == null) return -1;
    if (n == 0) return len;
    if (n > len) return -1;
    for (ssize_t i = len - n; i >= 0; i--) {
      if (memcmp(ptr + i, s, n * sizeof(T)) == 0) return i;
    }
    return -1;
  }

  auto rfind(T c) const -> ssize_t {
    for (ssize_t i = len - 1; i >= 0; i--) {
      if (ptr[i] == c) return i;
    }
    return -1;
  }

  auto contains(const BaseString &s) const -> bool {
    return find(s) >= 0;
  }

  auto contains(const std::string &s) const -> bool {
    return find(s) >= 0;
  }

  auto contains(const T *s) const -> bool {
    return find(s) >= 0;
  }

  auto contains(const T *s, size_t n) const -> bool {
    return find(s, n) >= 0;
  }

  auto contains(T c) const -> bool {
    return find(c) >= 0;
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
    return append(s);
  }

  auto operator+(const std::string &s) const -> BaseString {
    return append(s);
  }

  auto operator+(const T *s) const -> BaseString {
    return append(s);
  }

  auto operator+(T c) const -> BaseString {
    return append(c);
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

  auto operator+=(std::string &s) -> BaseString & {
    return *this = prepend(s);
  }

  auto operator+=(const T *s) -> BaseString & {
    return *this = prepend(s);
  }

  auto operator+=(T c) -> BaseString & {
    return *this = prepend(c);
  }

  friend auto operator+(const std::string &s, const BaseString &t) -> BaseString {
    return t.prepend(s);
  }

  friend auto operator+(const T *s, const BaseString &t) -> BaseString {
    return t.prepend(s);
  }

  friend auto operator+(T c, const BaseString &t) -> BaseString {
    return t.prepend(c);
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

template <typename T>
class BaseStringBuilder {
  struct Data {
    u32 _rc;     // 引用计数
    u32 _len;    // 数据长度
    T   _data[]; // 数据 (动态分配)
  };

  u32   len = 0;
  u32   cap = 0;
  Data *ptr = null;

#define data (this->ptr->_data)

  using BaseString = BaseString<T>;

public:
  BaseStringBuilder() = default;
  BaseStringBuilder(size_t cap) : cap(cap) {
    if (cap == 0) return;
    ptr = (Data *)malloc(sizeof(Data) + cap * sizeof(T));
  }
  ~BaseStringBuilder() {
    if (ptr != null) free(ptr);
  }

  auto setcap(size_t n) -> BaseStringBuilder & {
    static constexpr u32 cap_padding = 64;
    if (n <= cap) return *this;
    cap = (n & ~(cap_padding - 1)) + cap_padding;
    ptr = (Data *)realloc(ptr, sizeof(Data) + cap * sizeof(T));
    return *this;
  }

  auto empty() const -> bool {
    return len == 0;
  }

  auto length() const -> size_t {
    return len;
  }

  auto size() const -> size_t {
    return len * sizeof(T);
  }

  auto operator[](size_t i) const -> T {
    return data[i];
  }

  auto at(size_t i) const -> T {
    if (i >= len) return 0;
    return data[i];
  }

  auto append(const BaseString &s) -> BaseStringBuilder & {
    if (s.empty()) return *this;
    setcap(len + s.length());
    memcpy(data + len, s.getptr(), s.length() * sizeof(T));
    len += s.length();
    return *this;
  }

  auto append(const T *s, size_t n) -> BaseStringBuilder & {
    if (s == null || n == 0) return *this;
    setcap(len + n);
    memcpy(data + len, s, n * sizeof(T));
    len += n;
    return *this;
  }

  auto append(const T *s) -> BaseStringBuilder & {
    if (s == null) return *this;
    size_t n = strlen(s);
    if (n == 0) return *this;
    setcap(len + n);
    memcpy(data + len, s, n * sizeof(T));
    len += n;
    return *this;
  }

  auto append(const T &c) -> BaseStringBuilder & {
    setcap(len + 1);
    data[len++] = c;
    return *this;
  }

  auto operator+=(const BaseString &s) -> BaseStringBuilder & {
    return append(s);
  }

  auto operator+=(const T *s) -> BaseStringBuilder & {
    return append(s);
  }

  auto operator+=(const T &c) -> BaseStringBuilder & {
    return append(c);
  }

  // 输出为 string 并清空 builder 的内容
  auto str() -> BaseString {
    if (ptr == null) return {};
    if (len == cap) ptr = (Data *)realloc(ptr, sizeof(Data) + (cap + 1) * sizeof(T));
    ptr->_rc  = 0;
    ptr->_len = len;
    data[len] = 0;
    BaseString s{len, 0, data};
    len = 0, cap = 0, ptr = null;
    return s;
  }

#undef data
};

} // namespace lumos::base

namespace std {

template <typename T>
struct hash<lumos::base::BaseString<T>> {
  auto operator()(const lumos::base::BaseString<T> &s) const -> size_t {
    return s.hash();
  }
};

static template struct hash<lumos::base::BaseString<char>>;
static template struct hash<lumos::base::BaseString<wchar_t>>;
static template struct hash<lumos::base::BaseString<char16_t>>;
static template struct hash<lumos::base::BaseString<char32_t>>;

} // namespace std

using str        = lumos::base::BaseString<char>;
using strbuilder = lumos::base::BaseStringBuilder<char>;

using strref = const str &;

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

inline static auto println(strref s) {
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

  template <typename U>
  auto operator!=(const _P_<U> &p) const -> bool {
    return ptr != p.ptr;
  }

  template <typename U>
  auto operator!=(const U *p) const -> bool {
    return ptr != p;
  }

  auto operator!=(const std::nullptr_t &p) const -> bool {
    return ptr != p;
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

  friend auto operator<<(ostream &u, _P_<T> &p) -> ostream & {
    return u << p.ptr;
  }

  friend auto operator>>(istream &u, const _P_<T> &p) -> istream & {
    return u >> p.ptr;
  }

  friend auto operator>>(istream &u, _P_<T> &p) -> istream & {
    return u >> p.ptr;
  }

  operator bool() const {
    return ptr != null;
  }

  // operator T *() const {
  //   return ptr;
  // }

  // template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<U, T>>>
  // operator U *() const {
  //   return static_cast<U *>(ptr);
  // }

  auto get() -> T * {
    return ptr;
  }

  auto get() const -> const T * {
    return ptr;
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

class mpz : public mpz_class {
public:
  using mpz_class::mpz_class;
  mpz(str s, int base = 10) : mpz_class((std::string)s, base) {}
};

class mpq : public mpq_class {
public:
  using mpq_class::mpq_class;
  mpq(str s, int base = 10) : mpq_class((std::string)s, base) {}
};

class mpf : public mpf_class {
public:
  using mpf_class::mpf_class;
  mpf(str s, int base = 10) : mpf_class((std::string)s, base) {}
};

namespace lumos {

template <typename T>
class MatchList {
  Array<Vector<str>, 256> strs;
  Array<Vector<T>, 256>   vals;

public:
  MatchList() = default;

  auto find(strref s) const -> T * {
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

  void append(strref s, const T &v) {
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
    explicit Error(strref msg) : ::Error(name ": " + msg) {}                                       \
  };                                                                                               \
  /* 程序内部错误 */                                                                         \
  class Fail : public ::Error {                                                                    \
  public:                                                                                          \
    explicit Fail(strref msg) : ::Error("[fail] " name ": " + msg) {}                              \
  };
