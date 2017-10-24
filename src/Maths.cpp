#include "../include/Maths.hpp"

Vector normale(const Object &obj, const size_t iFace)
{
    const Face &f = obj.faces()[iFace];
    return Vector(obj.points()[f.s0], obj.points()[f.s1]) ^ Vector(obj.points()[f.s0], obj.points()[f.s2]);
}
