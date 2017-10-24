#include "../include/Face.hpp"

Face::Face() : s0(0), s1(0), s2(0) {}

Face::Face(const Face &f) : s0(f.s0), s1(f.s1), s2(f.s2) {}

Face::Face(const unsigned int s0, const unsigned int s1, const unsigned int s2)
    : s0(s0), s1(s1), s2(s2) {}

Face::~Face() {}

Face &Face::operator=(const Face &f) {
  s0 = f.s0;
  s1 = f.s1;
  s2 = f.s2;

  return *this;
}

std::ostream &operator<<(std::ostream &os, const Face &f) {
  return os << 3 << ' ' << f.s0 << ' ' << f.s1 << ' ' << f.s2;
}

std::istream &operator>>(std::istream &is, Face &f) {
  unsigned int numVertices;
  is >> numVertices;

  if (numVertices != 3)
    throw std::runtime_error("Can only read 3 vertices.");

  return is >> f.s0 >> f.s1 >> f.s2;
}
