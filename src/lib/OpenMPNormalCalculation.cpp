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
  ~Mutex() {
      if (omp_test_lock(&_m))
        omp_destroy_lock(&_m);
      else
        throw std::runtime_error("Mutex locked; can't destroyed it.");
  }

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
{
}

OpenMPNormalCalculation::OpenMPNormalCalculation(const Object& object,
                                                 const size_t threadsAmount)
  : OpenMPNormalCalculation(object)
{
  omp_set_num_threads(threadsAmount);
}

void
OpenMPNormalCalculation::calculate()
{
  const size_t facesCount = _object.faces().size();
  const size_t vertexCount = _object.vertices().size();

#ifdef OPENMPNORMALCALCULATION_USE_PRIVATE_VAR
  _faceNormal.clear();
  _vertexNormal.clear();
#else
  _faceNormal.resize(facesCount);
  _vertexNormal.resize(vertexCount);
#endif

  std::deque<Mutex> vMutex(vertexCount);
  std::deque<Vertex> sumVertex(vertexCount);
  std::deque<unsigned int> nbVertex(vertexCount, 0);

#pragma omp parallel default(none) shared(_object, vMutex, sumVertex, nbVertex)
  {
#ifdef OPENMPNORMALCALCULATION_USE_PRIVATE_VAR
      auto faceNormalPrivate = _faceNormal;
      auto vertexNormalPrivate = _vertexNormal;
#endif

    // itération 0 -> taille; taille -> 0; random : modif la performance
#pragma omp for schedule(static)
    for (size_t i = 0; i < facesCount; ++i) {
      Vector normal = Maths::normal(_object, i);

#ifdef OPENMPNORMALCALCULATION_USE_PRIVATE_VAR
     faceNormalPrivate.push_back(normal);
#else
       _faceNormal[i] = normal;
#endif

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
    for (size_t i = 0; i < vertexCount; ++i) {
      if (nbVertex[i] > 0) {
#ifdef OPENMPNORMALCALCULATION_USE_PRIVATE_VAR
          vertexNormalPrivate.push_back(sumVertex[i] / nbVertex[i]);
#else
          _vertexNormal[i] = sumVertex[i] / nbVertex[i];
#endif
      } else {
#ifdef OPENMPNORMALCALCULATION_USE_PRIVATE_VAR
          throw std::runtime_error("nbVertex = 0");
          vertexNormalPrivate.push_back(Vector());
#endif
      }
    }

#ifdef OPENMPNORMALCALCULATION_USE_PRIVATE_VAR
#pragma omp for schedule(static) ordered
    for (int i = 0; i < omp_get_num_threads(); ++i) {
#pragma omp ordered
      _faceNormal.insert(
        _faceNormal.end(), faceNormalPrivate.begin(), faceNormalPrivate.end());
      _vertexNormal.insert(_vertexNormal.end(),
                           vertexNormalPrivate.begin(),
                           vertexNormalPrivate.end());
    }
#endif
  }
}
