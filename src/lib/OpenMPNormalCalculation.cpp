#include <lib/Maths.hpp>
#include <lib/OpenMPNormalCalculation.hpp>

#include <omp.h>

using namespace lib;

OpenMPNormalCalculation::OpenMPNormalCalculation(const Object& object)
  : NormalCalculation(object)
{}

void
OpenMPNormalCalculation::calculate()
{
  _faceNormal.clear();
  _vertexNormal.clear();

  std::deque<Vertex> sumVertex(_object.vertices().size());
  std::deque<unsigned int> nbVertex(_object.vertices().size(), 0);

  // itération 0 -> taille; taille -> 0; random : modifi la performance

#pragma omp parallel shared(                                                   \
  _object, _faceNormal, _vertexNormal, sumVertex, nbVertex)
  {
#pragma omp for
    for (size_t i = 0; i < _object.faces().size(); ++i) {
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

#pragma omp for
    for (size_t i = 0; i < _object.vertices().size(); ++i)
      _vertexNormal.push_back(sumVertex[i] / nbVertex[i]);
  }
}
