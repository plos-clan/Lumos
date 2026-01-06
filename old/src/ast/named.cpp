#include <lumos.hpp>

namespace lumos::ast {

NamedType::NamedType(Container *parent, strref name, PType &type)
    : Named(parent, name), type(type) {}

} // namespace lumos::ast
