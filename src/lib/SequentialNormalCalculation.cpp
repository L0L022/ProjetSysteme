#include <lib/Maths.hpp>
#include <lib/SequentialNormalCalculation.hpp>

using namespace lib;

SequentialNormalCalculation::SequentialNormalCalculation(const Object& object)
  : NormalCalculation(object)
{}

void
SequentialNormalCalculation::calculate()
{
  _faceNormal.clear();
  _vertexNormal.clear();

  std::deque<Vertex> sumVertex(_object.vertices().size());
  std::deque<unsigned int> nbVertex(_object.vertices().size(), 0);

  for (size_t i = 0; i < _object.faces().size(); ++i) { //itération 0 -> taille; taille -> 0; random : modifi la performance
    Vector normal = Maths::normal(_object, i);
    _faceNormal.push_back(normal);

    const Face& f = _object.faces()[i];

    sumVertex[f.v0] += normal;
    ++nbVertex[f.v0];

    sumVertex[f.v1] += normal;
    ++nbVertex[f.v1];

    sumVertex[f.v2] += normal;
    ++nbVertex[f.v2];
  }

  for (size_t i = 0; i < _object.vertices().size(); ++i)
    _vertexNormal.push_back(sumVertex[i] / nbVertex[i]);
}
