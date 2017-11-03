#ifndef NORMALCALCULATION_HPP
#define NORMALCALCULATION_HPP

#include "Object.hpp"
#include <deque>

class NormalCalculation {
public:
  NormalCalculation(const Object &object);
  virtual ~NormalCalculation();

  const Object &object() const { return _object; }

  virtual void calculateFaceNormal() = 0;
  inline const std::deque<Vector> &faceNormal() const { return _faceNormal; }

  virtual void calculateVertexNormal() = 0;
  inline const std::deque<Vector> &vertexNormal() const { return _vertexNormal; }

protected:
  const Object &_object;
  std::deque<Vector> _faceNormal;
  std::deque<Vector> _vertexNormal;
};

#endif
