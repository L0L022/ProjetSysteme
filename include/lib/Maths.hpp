#ifndef LIB_MATHS_HPP
#define LIB_MATHS_HPP

#include "Object.hpp"

#include <vector>

namespace lib { namespace Maths {

long double
angle_between_vectors(const Vector& v1, const Vector& v2);

Vector
normal(const Object& obj, const size_t iFace);

template<typename T, typename ForwardIt>
T
average(ForwardIt first, ForwardIt last)
{
  T res;
  size_t count = 0;

  for (; first != last; ++first, ++count) res += *first;

  return res / count;
}

}} // namespace lib::Maths

#endif
