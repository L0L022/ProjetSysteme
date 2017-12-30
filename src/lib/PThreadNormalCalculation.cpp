#include <lib/Maths.hpp>
#include <lib/PThreadNormalCalculation.hpp>

#include <algorithm>
#include <pthread.h>
#include <thread>

using namespace lib;

class LockGuard;

class Mutex
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
  std::deque<Vertex>& sumVertex;
  std::deque<unsigned int>& nbVertex;
};

struct Material
{
  CommonMaterial* cM;
  size_t begin;
  size_t end;
  std::deque<Vector> faceNormalPrivate;
  std::deque<Vector> vertexNormalPrivate;
};

void*
normalCalc(void* arg)
{
  auto& m = *static_cast<Material*>(arg);
  auto& _object = m.cM->object;
  auto& vMutex = m.cM->vMutex;
  auto& sumVertex = m.cM->sumVertex;
  auto& nbVertex = m.cM->nbVertex;
  auto& faceNormalPrivate = m.faceNormalPrivate;

  for (size_t i = m.begin; i < m.end; ++i) {
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
  return NULL;
}

void*
meanCalc(void* arg)
{
  auto& m = *static_cast<Material*>(arg);
  auto& sumVertex = m.cM->sumVertex;
  auto& nbVertex = m.cM->nbVertex;
  auto& vertexNormalPrivate = m.vertexNormalPrivate;

  for (size_t i = m.begin; i < m.end; ++i)
    if (nbVertex[i] > 0)
      vertexNormalPrivate.push_back(sumVertex[i] / nbVertex[i]);

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
  clear();

  const size_t vertexCount = _object.vertices().size();
  std::deque<Mutex> vMutex(vertexCount);
  std::deque<Vertex> sumVertex(vertexCount);
  std::deque<unsigned int> nbVertex(vertexCount, 0);

  std::vector<pthread_t> threads(_threadsAmount);
  std::vector<Material> materials(_threadsAmount);

  CommonMaterial cM{ _object, vMutex, sumVertex, nbVertex };
  {
    size_t count = _object.faces().size();
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

  for (Material& m : materials) {
    _faceNormal.insert(_faceNormal.end(),
                       m.faceNormalPrivate.begin(),
                       m.faceNormalPrivate.end());
    _vertexNormal.insert(_vertexNormal.end(),
                         m.vertexNormalPrivate.begin(),
                         m.vertexNormalPrivate.end());
  }
}
