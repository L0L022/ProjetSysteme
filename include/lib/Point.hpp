#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>

class Vector;

class Point {
public:
  double x;
  double y;
  double z;

  Point();
  Point(const double x, const double y, const double z);
  Point(const Point &);
  Point(const Vector &);
  ~Point();

  bool operator==(const Point &) const;

  Point &operator=(const Point &);
  Point &operator=(const Vector &);

  Point operator+(const Point &) const;
  Point operator+(const Vector &) const;
  Point &operator+=(const Vector &);

  Point operator-(const Point &) const;

  Point operator*(const Point &)const;
  Point &operator*=(const Point &);

  Point operator*(const double)const;
  Point &operator*=(const double);

  Point operator/(const double) const;
  Point &operator/=(const double);

  Point operator/(const Point &) const;
  Point &operator/=(const Point &);

  friend std::ostream &operator<<(std::ostream &, const Point &);
  friend std::istream &operator>>(std::istream &, Point &);
};

#endif
