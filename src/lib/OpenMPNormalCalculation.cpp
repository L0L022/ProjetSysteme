#include <lib/Maths.hpp>
#include <lib/OpenMPNormalCalculation.hpp>

#include <omp.h>

using namespace lib;

// https://stackoverflow.com/questions/18669296/c-openmp-parallel-for-loop-alternatives-to-stdvector
// https://stackoverflow.com/questions/29247593/openmp-segfault

class LockGuard;

class Mutex
{
public:
  Mutex() { omp_init_lock(&_m); }
  ~Mutex() { omp_destroy_lock(&_m); }

private:
  omp_lock_t _m;

  friend LockGuard;
};

class LockGuard
{
public:
  LockGuard(Mutex& m)
    : _m(m)
  {
    omp_set_lock(&_m._m);
  }
  ~LockGuard() { omp_unset_lock(&_m._m); }

private:
  Mutex& _m;
};

OpenMPNormalCalculation::OpenMPNormalCalculation(const Object& object)
  : NormalCalculation(object)
{}

OpenMPNormalCalculation::OpenMPNormalCalculation(const Object& object, const size_t threadsAmount)
: OpenMPNormalCalculation(object)
{
  omp_set_num_threads(threadsAmount);
}

void
OpenMPNormalCalculation::calculate()
{
  _faceNormal.clear();
  _vertexNormal.clear();

  const size_t vertexCount = _object.vertices().size();
  std::deque<Mutex> vMutex(vertexCount);

#pragma omp parallel default(none) shared(_object, vMutex)
  {
    auto faceNormalPrivate = _faceNormal;
    auto vertexNormalPrivate = _vertexNormal;

    std::deque<Vertex> sumVertex(vertexCount);
    std::deque<unsigned int> nbVertex(vertexCount, 0);

    // itération 0 -> taille; taille -> 0; random : modif la performance
#pragma omp for schedule(static)
    for (size_t i = 0; i < _object.faces().size(); ++i) {
      Vector normal = Maths::normal(_object, i);
      faceNormalPrivate.push_back(normal);

      const Face& f = _object.faces()[i];
      {
        LockGuard lg(vMutex[f.v0]);
        sumVertex[f.v0] += normal;
        ++nbVertex[f.v0];
      }
      {
        LockGuard lg(vMutex[f.v1]);
        sumVertex[f.v1] += normal;
        ++nbVertex[f.v1];
      }
      {
        LockGuard lg(vMutex[f.v2]);
        sumVertex[f.v2] += normal;
        ++nbVertex[f.v2];
      }
    }

#pragma omp for schedule(static)
    for (size_t i = 0; i < vertexCount; ++i)
      if (nbVertex[i] > 0)
        vertexNormalPrivate.push_back(sumVertex[i] / nbVertex[i]);

#pragma omp for schedule(static) ordered
    for (int i = 0; i < omp_get_num_threads(); ++i) {
#pragma omp ordered
      _faceNormal.insert(
        _faceNormal.end(), faceNormalPrivate.begin(), faceNormalPrivate.end());
      _vertexNormal.insert(_vertexNormal.end(),
                           vertexNormalPrivate.begin(),
                           vertexNormalPrivate.end());
    }
  }
}
