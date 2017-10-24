#include <iostream>
#include <string>

#include "../include/Maths.hpp"
#include "../include/Object.hpp"

Point VecteurFromPoints(const Point *_p1, const Point *_p2) {
  Point res;
  res.x = _p2->x - _p1->x;
  res.y = _p2->y - _p1->y;
  res.z = _p2->z - _p1->z;
  return res;
}

Object::Object() {}

Object::~Object() {}

Object Object::readOFF(std::istream &is) {
  std::string magic;
  is >> magic;

  if (magic != "OFF")
    throw std::runtime_error("Wrong magic number");

  Object obj;
  size_t arraySize;
  is >> arraySize;
  obj._points.resize(arraySize);
  is >> arraySize;
  obj._faces.resize(arraySize);
  is >> arraySize;

  for (Point &p : obj._points) {
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
  os << "OFF\n" << _points.size() << ' ' << _faces.size() << ' ' << 0 << '\n';
  for (const Point &p : _points)
    os << p << '\n';
  for (const Face &f : _faces)
    os << f << '\n';
}

void Object::writeOBJ(std::ostream &os) const {
  for (const Point &p : _points)
    os << "v " << p << '\n';
  for (int i = 0; i < _faces.size(); ++i)
    os << "vn " << Point(normale(*this, i).normalize(1)) << '\n';
  for (int i = 0; i < _faces.size(); ++i) {
      const Face &f = _faces[i];
    os << "f " << f.s0+1 << "//" << i+1 << ' ' << f.s1+1  << "//" << i+1<< ' ' << f.s2+1  << "//" << i+1<< '\n';
}
  //   os << "f " << f.s0 << "\\\\1" << ' ' << f.s1 << "\\\\1" << ' ' << f.s2 <<
  //   "\\\\1" << '\n';
}
