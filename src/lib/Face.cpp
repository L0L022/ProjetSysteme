#include <lib/Face.hpp>

Face::Face() : v0(0), v1(0), v2(0) {}

Face::Face(const Face &f) : v0(f.v0), v1(f.v1), v2(f.v2) {}

Face::Face(const size_t v0, const size_t v1, const size_t v2)
    : v0(v0), v1(v1), v2(v2) {}

Face &Face::operator=(const Face &f) {
  v0 = f.v0;
  v1 = f.v1;
  v2 = f.v2;

  return *this;
}

std::ostream &operator<<(std::ostream &os, const Face &f) {
  return os << 3 << ' ' << f.v0 << ' ' << f.v1 << ' ' << f.v2;
}

std::istream &operator>>(std::istream &is, Face &f) {
  size_t numVertices;
  is >> numVertices;

  if (numVertices != 3)
    throw std::runtime_error("Can only read 3 vertices.");

  return is >> f.v0 >> f.v1 >> f.v2;
}
