#include <lumos.hpp>

namespace lumos {

auto CTX::loadfile(str path) -> File * {
  path = std::filesystem::absolute((std::string)path);
  if (auto it = files.find(path); it != files.end()) return it->second;
  std::ifstream fs(path, std::ios::binary | std::ios::ate);
  if (!fs.is_open()) return null;
  size_t size = fs.tellg();
  fs.seekg(0, std::ios::beg);
  auto data = malloc(size + 1);
  if (!fs.read((char *)data, size)) {
    free(data);
    return null;
  }
  ((char *)data)[size] = '\0';
  fs.close();
  auto file   = new File{path, data, size};
  files[path] = file;
  return file;
}

auto storefile(CTX &ctx, str path, void *data, size_t size) -> File * {
  path = std::filesystem::absolute((std::string)path);
  if (auto it = ctx.files.find(path); it != ctx.files.end()) return it->second;
  auto file       = new File{path, data, size};
  ctx.files[path] = file;
  return file;
}

} // namespace lumos
