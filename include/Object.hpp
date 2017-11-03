#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <deque>
#include <iostream>

#include "Face.hpp"
#include "Point.hpp"

class Object {
public:
  Object() = default;
  Object(const Object &) = default;
  ~Object() = default;

  inline const std::deque<Point> &vertices() const { return _vertices; }
  inline const std::deque<Face> &faces() const { return _faces; }

  inline const Point &min() const { return _min; }
  inline const Point &max() const { return _max; }
  inline const Point &centroid() const { return _centroid; }

  static Object readOFF(std::istream &);
  void writeOFF(std::ostream &) const;

  void writeOBJ(std::ostream &) const;

private:
  std::deque<Point> _vertices;
  std::deque<Face> _faces;

  Point _min;
  Point _max;
  Point _centroid;
};

#endif
