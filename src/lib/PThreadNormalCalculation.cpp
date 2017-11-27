#include <lib/Maths.hpp>
#include <lib/PThreadNormalCalculation.hpp>

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

struct TMaterial {
  std::deque<Mutex>& vMutex;
  std::deque<Vertex>& sumVertex;
  std::deque<unsigned int>& nbVertex;
  std::deque<Vector> faceNormalPrivate;
  std::deque<Vector> vertexNormalPrivate;
};

PThreadNormalCalculation::PThreadNormalCalculation(const Object& object)
  : PThreadNormalCalculation(object, std::thread::hardware_concurrency())
{}

PThreadNormalCalculation::PThreadNormalCalculation(const Object& object, const size_t threadsAmount)
: NormalCalculation(object), _threadsAmount(threadsAmount)
{}

void
PThreadNormalCalculation::calculate()
{
  _faceNormal.clear();
  _vertexNormal.clear();

  const size_t vertexCount = _object.vertices().size();
  std::deque<Mutex> vMutex(vertexCount);
  std::deque<Vertex> sumVertex(vertexCount);
  std::deque<unsigned int> nbVertex(vertexCount, 0);

  std::vector<pthread_t> threads;
  std::vector<TMaterial> tMaterials;

  for (size_t i = 0; i < _threadsAmount; ++i) {
    tMaterials.push_back({vMutex, sumVertex, nbVertex});
    TMaterial& m = tMaterials.back();
    threads.emplace_back();
    pthread_t& t = threads.back();

    pthread_create(&t, NULL, PThreadNormalCalculation::threadCalculate, &m);
  }

  for (size_t i = 0; i < _threadsAmount; i++)
    pthread_join(&threads[i]);
}

void*
PThreadNormalCalculation::threadCalculate(void* arg)
{
  TMaterial& m = *static_cast<TMaterial*>(arg);
}
