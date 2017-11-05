#include <lib/Maths.hpp>

#include <lib/Face.hpp>
#include <lib/Vector.hpp>

Vector
Maths::normal(const Object& obj, const size_t iFace)
{
  const Face& f = obj.faces()[iFace];
  const Vector& v0 = obj.vertices()[f.v0];
  return Vector(v0, obj.vertices()[f.v1]) ^ Vector(v0, obj.vertices()[f.v2]);
}
