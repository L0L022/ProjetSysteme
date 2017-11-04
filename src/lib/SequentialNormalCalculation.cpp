#include <lib/Maths.hpp>
#include <lib/SequentialNormalCalculation.hpp>

SequentialNormalCalculation::SequentialNormalCalculation(const Object &object)
    : NormalCalculation(object) {}

void SequentialNormalCalculation::calculate() {
  _faceNormal.clear();
  _vertexNormal.clear();

  std::deque<Point> sumPoint(_object.vertices().size());
  std::deque<unsigned int> nbPoint(_object.vertices().size(), 0);

  for (size_t i = 0; i < _object.faces().size(); ++i) {
    Vector normal = Maths::normal(_object, i);
    _faceNormal.push_back(normal);

    const Face &f = _object.faces()[i];

    sumPoint[f.v0] += normal;
    ++nbPoint[f.v0];

    sumPoint[f.v1] += normal;
    ++nbPoint[f.v1];

    sumPoint[f.v2] += normal;
    ++nbPoint[f.v2];
  }

  for (size_t i = 0; i < _object.vertices().size(); ++i)
    _vertexNormal.push_back(sumPoint[i] / nbPoint[i]);
}
