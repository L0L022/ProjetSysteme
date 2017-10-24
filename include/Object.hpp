#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <deque>
#include <iostream>

#include "Face.hpp"
#include "Point.hpp"

class Object {
public:
  // typedef std::deque std::deque;

  Object();
  Object(const Object &);
  ~Object();

  //renomer en vertices
  inline const std::deque<Point> &points() const { return _points; }
  inline const std::deque<Face> &faces() const { return _faces; }

  inline const Point &min() const { return _min; }
  inline const Point &max() const { return _max; }
  inline const Point &centroid() const { return _centroid; }

  static Object readOFF(std::istream &);
  void writeOFF(std::ostream &) const;

  void writeOBJ(std::ostream &) const;

private:
  std::deque<Point> _points;
  std::deque<Face> _faces;

  Point _min;
  Point _max;
  Point _centroid;
};

#endif
