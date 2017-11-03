#include <iostream>
#include <string>

#include "../include/Maths.hpp"
#include "../include/Object.hpp"

Object Object::readOFF(std::istream &is) {
  std::string magic;
  is >> magic;

  if (magic != "OFF")
    throw std::runtime_error("Wrong magic number");

  Object obj;
  size_t arraySize;
  is >> arraySize;
  obj._vertices.resize(arraySize);
  is >> arraySize;
  obj._faces.resize(arraySize);
  is >> arraySize;

  for (Point &p : obj._vertices) {
    is >> p;

    if (obj._min.x > p.x)
      obj._min.x = p.x;
    if (obj._min.y > p.y)
      obj._min.y = p.y;
    if (obj._min.z > p.z)
      obj._min.z = p.z;

    if (obj._max.x < p.x)
      obj._max.x = p.x;
    if (obj._max.y < p.y)
      obj._max.y = p.y;
    if (obj._max.z < p.z)
      obj._max.z = p.z;
  }

  for (Face &f : obj._faces)
    is >> f;

  obj._centroid.x = (obj._max.x + obj._min.x) / 2.0;
  obj._centroid.y = (obj._max.y + obj._min.y) / 2.0;
  obj._centroid.z = (obj._max.z + obj._min.z) / 2.0;

  return obj;
}

void Object::writeOFF(std::ostream &os) const {
  os << "OFF\n" << _vertices.size() << ' ' << _faces.size() << ' ' << 0 << '\n';
  for (const Point &p : _vertices)
    os << p << '\n';
  for (const Face &f : _faces)
    os << f << '\n';
}

void Object::writeOBJ(std::ostream &os) const {
  for (const Point &p : _vertices)
    os << "v " << p << '\n';
  for (size_t i = 0; i < _faces.size(); ++i)
    os << "vn " << Point(normale(*this, i).normalize(1)) << '\n';
  for (size_t i = 0; i < _faces.size(); ++i) {
    const Face &f = _faces[i];
    os << "f " << f.v0+1 << "//" << i+1 << ' ' << f.v1+1  << "//" << i+1<< ' ' << f.v2+1  << "//" << i+1<< '\n';
  }
}
