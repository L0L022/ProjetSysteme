#include <lib/Maths.hpp>
#include <lib/OpenMPNormalCalculation.hpp>

#include <omp.h>

using namespace lib;

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
  _faceNormal.resize(facesCount);
  _vertexNormal.resize(vertexCount);

  std::deque<Mutex> vMutex(vertexCount);

#pragma omp parallel default(none)                                             \
  shared(_object, vMutex, _faceNormal, _vertexNormal)
  {

#pragma omp for schedule(static)
    for (size_t i = 0; i < facesCount; ++i) {
      const Face& f = _object.faces()[i];
      const Vertex &v0 = _object.vertices()[f.v0],
                   &v1 = _object.vertices()[f.v1],
                   &v2 = _object.vertices()[f.v2];
      Vector normal = Vector(v0, v1) ^ Vector(v0, v2);

      if (!(_faceNormal[i] == Vector())) {
        {
          LockGuard lg(vMutex[f.v0]);
          _vertexNormal[f.v0] +=
            normal * Maths::angle_between_vectors(v0 - v1, v0 - v2);
        }
        {
          LockGuard lg(vMutex[f.v1]);
          _vertexNormal[f.v1] +=
            normal * Maths::angle_between_vectors(v1 - v0, v1 - v2);
        }
        {
          LockGuard lg(vMutex[f.v2]);
          _vertexNormal[f.v2] +=
            normal * Maths::angle_between_vectors(v2 - v0, v2 - v1);
        }
      }

      _faceNormal[i] = normal.normalize(1);
    }

#pragma omp for schedule(static)
    for (size_t i = 0; i < vertexCount; ++i) {
      _vertexNormal[i].normalize(1);
    }
  }
}
