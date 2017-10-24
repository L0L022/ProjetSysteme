#ifndef MATHS_HPP
#define MATHS_HPP

#include "Face.hpp"
#include "Object.hpp"
#include "Vector.hpp"

#include <vector>

Vector normale(const Object &obj, const size_t iFace);

template <typename T, typename ForwardIt>
T average(ForwardIt first, ForwardIt last) {
  T res;
  size_t count = 0;

  for (; first != last; ++first, ++count)
    res += *first;

  return res / count;
}

#endif
