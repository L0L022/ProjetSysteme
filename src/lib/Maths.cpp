#include <lib/Maths.hpp>

#include <lib/Face.hpp>
#include <lib/Vector.hpp>

#include <cmath>
#include <iostream>

using namespace lib;

long double
Maths::angle_between_vectors(const Vector& v1, const Vector& v2)
{
  long double t = v1.length() * v2.length();
  if (t == 0.0) throw std::runtime_error("Division by zero.");
  return std::acos((v1 * v2) / t);
}

Vector
Maths::normal(const Object& obj, const size_t iFace)
{
  const Face& f = obj.faces()[iFace];
  const Vector& v0 = obj.vertices()[f.v0];
  return Vector(v0, obj.vertices()[f.v1]) ^
         Vector(v0, obj.vertices()[f.v2]).normalize(1);
}
