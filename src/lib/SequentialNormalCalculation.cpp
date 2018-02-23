#include <lib/Maths.hpp>
#include <lib/SequentialNormalCalculation.hpp>

using namespace lib;

#include <iostream>

SequentialNormalCalculation::SequentialNormalCalculation(const Object& object)
  : NormalCalculation(object)
{
}

void
SequentialNormalCalculation::calculate()
{
  const size_t facesCount = _object.faces().size();
  const size_t vertexCount = _object.vertices().size();
  _faceNormal.resize(facesCount);
  _vertexNormal.resize(vertexCount);

  // itération 0 -> taille; taille -> 0; random : modifi la performance
  for (size_t i = 0; i < facesCount; ++i) {
    const Face& f = _object.faces()[i];
    const Vertex &v0 = _object.vertices()[f.v0], &v1 = _object.vertices()[f.v1],
                 &v2 = _object.vertices()[f.v2];

    _faceNormal[i] = Vector(v0, v1) ^ Vector(v0, v2);

    if (!(_faceNormal[i] == Vector())) {
      _vertexNormal[f.v0] +=
        _faceNormal[i] * Maths::angle_between_vectors(v0 - v1, v0 - v2);
      _vertexNormal[f.v1] +=
        _faceNormal[i] * Maths::angle_between_vectors(v1 - v0, v1 - v2);
      _vertexNormal[f.v2] +=
        _faceNormal[i] * Maths::angle_between_vectors(v2 - v0, v2 - v1);
    }

    _faceNormal[i].normalize(1);
  }

  for (size_t i = 0; i < vertexCount; ++i) {
    _vertexNormal[i].normalize(1);
  }
}
