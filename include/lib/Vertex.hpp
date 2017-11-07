#ifndef LIB_VERTEX_HPP
#define LIB_VERTEX_HPP

#include <iostream>

namespace lib {

class Vector;

class Vertex
{
public:
  double x;
  double y;
  double z;

  Vertex();
  Vertex(const double x, const double y, const double z);
  Vertex(const Vertex&);
  Vertex(const Vector&);
  ~Vertex();

  bool operator==(const Vertex&) const;

  Vertex& operator=(const Vertex&);
  Vertex& operator=(const Vector&);

  Vertex operator+(const Vertex&) const;
  Vertex operator+(const Vector&) const;
  Vertex& operator+=(const Vector&);

  Vertex operator-(const Vertex&) const;

  Vertex operator*(const Vertex&)const;
  Vertex& operator*=(const Vertex&);

  Vertex operator*(const double)const;
  Vertex& operator*=(const double);

  Vertex operator/(const double) const;
  Vertex& operator/=(const double);

  Vertex operator/(const Vertex&) const;
  Vertex& operator/=(const Vertex&);

  friend std::ostream& operator<<(std::ostream&, const Vertex&);
  friend std::istream& operator>>(std::istream&, Vertex&);
};

} // namespace lib

#endif
