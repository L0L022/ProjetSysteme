#ifndef NORMALCALCULATION_HPP
#define NORMALCALCULATION_HPP

#include "Object.hpp"
#include "Vector.hpp"
#include <deque>

namespace lib {

class NormalCalculation
{
public:
  NormalCalculation(const Object& object);
  virtual ~NormalCalculation();

  inline const Object& object() const { return _object; }
  inline const std::deque<Vector>& faceNormal() const { return _faceNormal; }
  inline const std::deque<Vector>& vertexNormal() const
  {
    return _vertexNormal;
  }

  virtual void calculate() = 0;

protected:
  const Object& _object;
  std::deque<Vector> _faceNormal;
  std::deque<Vector> _vertexNormal;
};

}

#endif
