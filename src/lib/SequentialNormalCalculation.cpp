#include <lib/Maths.hpp>
#include <lib/SequentialNormalCalculation.hpp>

using namespace lib;

SequentialNormalCalculation::SequentialNormalCalculation(const Object& object)
  : NormalCalculation(object)
{
}

void
SequentialNormalCalculation::calculate()
{
  //clear();

  const size_t facesCount = _object.faces().size();
  const size_t vertexCount = _object.vertices().size();
  _faceNormal.resize(facesCount);
  _vertexNormal.resize(vertexCount);

  std::deque<Vertex> sumVertex(vertexCount);
  std::deque<unsigned int> nbVertex(vertexCount, 0);

  // itération 0 -> taille; taille -> 0; random : modifi la performance
  for (size_t i = 0; i < facesCount; ++i) {
    Vector normal = Maths::normal(_object, i);
    _faceNormal[i] = normal;

    const Face& f = _object.faces()[i];

    sumVertex[f.v0] += normal;
    ++nbVertex[f.v0];

    sumVertex[f.v1] += normal;
    ++nbVertex[f.v1];

    sumVertex[f.v2] += normal;
    ++nbVertex[f.v2];
  }

  for (size_t i = 0; i < vertexCount; ++i) {
    if (nbVertex[i] > 0) _vertexNormal[i] = sumVertex[i] / nbVertex[i];
  }
}
