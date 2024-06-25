#pragma once

#include <type.hpp>

namespace lumos::io {

class IFile {
  str    name;             // 文件名
  str    path;             // 路径
  void  *data     = null;  // 数据，读取的或 mmap
  size_t size     = 0;     // 数据的大小 (不包括尾部填充的二进制0)
  bool   once     = false; // 是否只导入一次
  u32    included = 0;     // 该文件被

public:
  IFile(str path);
  ~IFile();
};
class OFile {};

auto open_for_read(str path) -> IFile *;
auto open_for_write(str path) -> OFile *;

} // namespace lumos::io

namespace lumos {
using io::open_for_read;
using io::open_for_write;
} // namespace lumos
