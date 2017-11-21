#include <algorithm>
#include <fstream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <lib/Maths.hpp>
#include <lib/SequentialNormalCalculation.hpp>
#include <lib/OpenMPNormalCalculation.hpp>
#include <lib/Object.hpp>
#include <lib/Vector.hpp>

#include <benchmark/benchmark.h>

using namespace std;
using namespace lib;

// struct ArgsFileNormalCalculation
// {
//   std::string fileName;
//   lib::NormalCalculation::Method method;
// };
//
// template<class... ExtraArgs>
// static void
// BM_FileNormalCalculation(benchmark::State& state, ExtraArgs&&... extra_args)
// {
//   ArgsFileNormalCalculation args = { extra_args... };
//
//   ifstream file(args.fileName, std::ios::binary);
//   if (!file.is_open()) {
//     state.SkipWithError("Can't open OFF file.");
//     return;
//   }
//   stringstream ss;
//   ss << file.rdbuf();
//
//   Object obj = Object::readOFF(ss);
//
//   shared_ptr<NormalCalculation> calc(
//     NormalCalculation::factory(args.method, obj));
//   if (!calc) state.SkipWithError("Can't make NormalCalculation instance.");
//
//   for (auto _ : state) calc->calculate();
// }
//
// // BENCHMARK_CAPTURE(BM_FileNormalCalculation,
// //                   Sequential,
// //                   "bouddha1m.off",
// //                   NormalCalculation::Method::Sequential);
//
// template<class... ExtraArgs>
// static void
// BM_NormalCalculation(benchmark::State& state, ExtraArgs&&... extra_args)
// {
//   Object obj = Object::randGen(state.range(0), state.range(0));
//   auto method = NormalCalculation::Method::Sequential;
//   shared_ptr<NormalCalculation> calc(
//     NormalCalculation::factory(extra_args..., obj));
//   if (!calc) state.SkipWithError("Can't make NormalCalculation instance.");
//
//   for (auto _ : state) calc->calculate();
// }
//
// static void
// ObjectRandArgs(benchmark::internal::Benchmark* b)
// {
//   b->Unit(benchmark::kMillisecond);
//   // for (int i = 50; i <= 1'000'000; i += 199990) b->Args({ i / 2, i });
//   // for (int j = 25; j <= 500'000; j += 99995) b->Args({ i, j });
//   for (int i :
//        { 0, 100'000, 500'000, 1'000'000, 2'000'000, 5'000'000, 10'000'000 })
//     b->Args({ i / 2, i });
// }
//
// BENCHMARK_CAPTURE(BM_NormalCalculation,
//                   Sequential,
//                   NormalCalculation::Method::Sequential)
//   ->Apply(ObjectRandArgs);
// // BENCHMARK_CAPTURE(BM_NormalCalculation, pThread,
// // NormalCalculation::Method::pThread);
// BENCHMARK_CAPTURE(BM_NormalCalculation,
//                   OpenMP,
//                   NormalCalculation::Method::OpenMP)
//   ->Apply(ObjectRandArgs);

class ObjectLoader
{
public:
  ObjectLoader(lib::Object & object) : _obj(object) {}
  virtual ~ObjectLoader() {};
  virtual void load() = 0;

protected:
  lib::Object& _obj;
};

class RandObjectLoader : public ObjectLoader
{
public:
  const size_t nbVertices;
  const size_t nbFaces;

  RandObjectLoader(lib::Object& object, const size_t nbVertices, const size_t nbFaces)
    : ObjectLoader(object), nbVertices(nbVertices), nbFaces(nbFaces) {}

  void load() {
    // if (_obj.)
    _obj = Object::randGen(nbVertices, nbFaces);
  }
};

struct BM_NormalCalculationArgs {
  std::shared_ptr<ObjectLoader> objLoader;
  std::shared_ptr<NormalCalculation> calc;
};

auto BM_NormalCalculation = [](benchmark::State& state, auto Inputs)
{
  BM_NormalCalculationArgs args(Inputs);
  args.objLoader->load();
  for (auto _ : state) args.calc->calculate();
};

// class get object, get() -> genObject static

int main(int argc, char** argv) {

  //Random object
  std::deque<Object> objs;
  for (int nbFaces : { 0, 100'000, 500'000, 1'000'000, 2'000'000, 5'000'000, 10'000'000 }) {
    int nbVertices = nbFaces / 2;
    std::string name("Rand/F:");
    name += std::to_string(nbFaces);
    name += "/V:";
    name += std::to_string(nbVertices);

    objs.emplace_back();
    Object & obj = objs.back();

    BM_NormalCalculationArgs args;
    args.objLoader = std::make_shared<RandObjectLoader>(obj, nbVertices, nbFaces);

    {
      args.calc = std::make_shared<SequentialNormalCalculation>(obj);
      benchmark::RegisterBenchmark((name+"/Sequential").c_str(), BM_NormalCalculation, args);
    }
    {
      for (int nbThreads : {2, 3, 4, 8}) {
        args.calc = std::make_shared<OpenMPNormalCalculation>(obj, nbThreads);
        benchmark::RegisterBenchmark((name+"/OpenMP/"+std::to_string(nbThreads)).c_str(), BM_NormalCalculation, args);
      }
    }
  }

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}
