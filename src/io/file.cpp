#include <cstdlib>
#include <lumos/io.hpp>

namespace lumos::io {

IFile::IFile(str path) {
  cstr abspath = realpath(path.c_str(), null);
  this->path   = str(abspath);
  this->name   = str(basename(abspath));
  free((void *)abspath);
}
IFile::~IFile() {
  free(data);
}

auto open_for_read(str path) -> IFile * {}

auto open_for_write(str path) -> OFile * {}

} // namespace lumos::io