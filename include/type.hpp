#pragma once

#include <array>
#include <ccomplex>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
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

using str      = std::string;
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

class RefCount {
public:
  size_t __rc__ = 0;
};

template <typename T>
class __P__ {
#define __rc__ (((RefCount *)ptr)->__rc__)

public:
  T *ptr = null;

  __P__() = default;

  __P__(T *p) : ptr(p) {
    if (ptr) __rc__++;
  }

  template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<T, U>>>
  __P__(U *p) : ptr(static_cast<T *>(p)) {
    if (ptr) __rc__++;
  }

  __P__(const __P__<T> &p) : ptr(p.ptr) {
    if (ptr) __rc__++;
  }

  template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<T, U>>>
  __P__(const __P__<U> &p) : ptr(static_cast<T *>(p.ptr)) {
    if (ptr) __rc__++;
  }

  __P__(__P__<T> &&p) noexcept : ptr(p.ptr) {
    p.ptr = null;
  }

  template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<T, U>>>
  __P__(__P__<U> &&p) noexcept : ptr(p.ptr) {
    p.ptr = null;
  }

  auto operator=(const __P__<T> &p) -> __P__<T> & {
    if (this == &p) return *this;
    if (ptr != null && --__rc__ == 0) delete ptr;
    ptr = p.ptr;
    __rc__++;
    return *this;
  }

  template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<T, U>>>
  auto operator=(const __P__<U> &p) -> __P__<T> & {
    if (this == &p) return *this;
    if (ptr != null && --__rc__ == 0) delete ptr;
    ptr = static_cast<T *>(p.ptr);
    __rc__++;
    return *this;
  }

  auto operator=(__P__<T> &&p) noexcept -> __P__<T> & {
    if (this == &p) return *this;
    if (ptr != null && --__rc__ == 0) delete ptr;
    ptr   = p.ptr;
    p.ptr = null;
    return *this;
  }

  template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<T, U>>>
  auto operator=(__P__<U> &&p) noexcept -> __P__<T> & {
    if (this == &p) return *this;
    if (ptr != null && --__rc__ == 0) delete ptr;
    ptr   = static_cast<T *>(p.ptr);
    p.ptr = null;
    return *this;
  }

  ~__P__() {
    if (ptr != null && --__rc__ == 0) delete ptr;
  }

  auto operator*() const -> T & {
    return *ptr;
  }

  auto operator->() const -> T * {
    return ptr;
  }

  operator T *() const {
    return ptr;
  }

  template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<U, T>>>
  operator U *() const {
    return static_cast<U *>(ptr);
  }

#undef __rc__
};

#define __Pclass__(__name__)                                                                       \
  class __name__;                                                                                  \
  using P##__name__ = __P__<__name__>
#define __P__(__name__) using P##__name__ = __P__<__name__>

template <typename T1, typename T2>
static inline auto isinstance(const T2 *ptr) -> bool {
  static_assert(std::is_base_of_v<T2, T1>);
  return dynamic_cast<const T1 *>(ptr) != null;
}
