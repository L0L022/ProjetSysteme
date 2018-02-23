#include <lib/Maths.hpp>
#include <lib/PThreadNormalCalculation.hpp>

#include <algorithm>
#include <pthread.h>
#include <thread>

using namespace lib;

class LockGuard;

class Mutex // permet de verouiller des données pour éviter des "colisions"
            // lorsque 2 threads travaillent simultanément sur la même variable.
{
public:
  Mutex() { pthread_mutex_init(&_m, NULL); }
  ~Mutex() { pthread_mutex_destroy(&_m); }

private:
  pthread_mutex_t _m;

  friend LockGuard;
};

class LockGuard
{
public:
  LockGuard(Mutex& m)
    : _m(m)
  {
    pthread_mutex_lock(&_m._m);
  }
  ~LockGuard() { pthread_mutex_unlock(&_m._m); }

private:
  Mutex& _m;
};

struct CommonMaterial
{
  const Object& object;
  std::deque<Mutex>& vMutex;
  std::deque<Vector>& faceNormal;
  std::deque<Vector>& vertexNormal;
};

struct Material
{
  CommonMaterial* cM;
  size_t begin;
  size_t end;
};

void*
normalCalc(void* arg)
{
  auto& m = *static_cast<Material*>(arg);
  auto& _object = m.cM->object;
  auto& vMutex = m.cM->vMutex;
  auto& _faceNormal = m.cM->faceNormal;
  auto& _vertexNormal = m.cM->vertexNormal;

  for (size_t i = m.begin; i < m.end; ++i) {
    const Face& f = _object.faces()[i];
    const Vertex &v0 = _object.vertices()[f.v0], &v1 = _object.vertices()[f.v1],
                 &v2 = _object.vertices()[f.v2];
    Vector normal = Vector(v0, v1) ^ Vector(v0, v2);

    if (!(normal == Vector())) {
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

  return NULL;
}

void*
meanCalc(void* arg)
{
  auto& m = *static_cast<Material*>(arg);
  auto& _vertexNormal = m.cM->vertexNormal;

  for (size_t i = m.begin; i < m.end; ++i) _vertexNormal[i].normalize(1);

  return NULL;
}

PThreadNormalCalculation::PThreadNormalCalculation(const Object& object)
  : PThreadNormalCalculation(object, std::thread::hardware_concurrency())
{
}

PThreadNormalCalculation::PThreadNormalCalculation(const Object& object,
                                                   const size_t threadsAmount)
  : NormalCalculation(object)
  , _threadsAmount(threadsAmount)
{
}

void
PThreadNormalCalculation::calculate()
{
  const size_t facesCount = _object.faces().size();
  const size_t vertexCount = _object.vertices().size();

  _faceNormal.resize(facesCount);
  _vertexNormal.resize(vertexCount);

  std::deque<Mutex> vMutex(vertexCount);

  std::vector<pthread_t> threads(_threadsAmount);
  std::vector<Material> materials(_threadsAmount);

  CommonMaterial cM{ _object, vMutex, _faceNormal, _vertexNormal };
  {
    size_t count = facesCount;
    size_t work = count / _threadsAmount;
    for (size_t i = 0; i < _threadsAmount; ++i) {
      Material& m = materials.at(i);
      m.cM = &cM;
      m.begin = i * work;
      m.end = std::min(i * work + work, count);
      pthread_create(&threads.at(i), NULL, normalCalc, &m);
    }

    for (pthread_t& t : threads) pthread_join(t, NULL);
  }
  {
    size_t count = vertexCount;
    size_t work = count / _threadsAmount;
    for (size_t i = 0; i < _threadsAmount; ++i) {
      Material& m = materials.at(i);
      m.cM = &cM;
      m.begin = i * work;
      m.end = std::min(i * work + work, count);
      pthread_create(&threads.at(i), NULL, meanCalc, &m);
    }

    for (pthread_t& t : threads) pthread_join(t, NULL);
  }
}
