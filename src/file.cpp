#include <lumos.hpp>

namespace lumos {

auto loadfile(Ctx &ctx, str path) -> File * {
  path = std::filesystem::absolute(path);
  if (auto it = ctx.files.find(path); it != ctx.files.end()) return it->second;
  std::ifstream fs(path, std::ios::binary | std::ios::ate);
  if (!fs.is_open()) return null;
  size_t size = fs.tellg();
  fs.seekg(0, std::ios::beg);
  auto data = malloc(size);
  if (!fs.read((char *)data, size)) {
    free(data);
    return null;
  }
  fs.close();
  auto file       = new File{path, data, size};
  ctx.files[path] = file;
  return file;
}

} // namespace lumos
