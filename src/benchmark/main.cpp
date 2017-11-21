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

static void
benchmarkApplyArgs(benchmark::internal::Benchmark* b)
{
  b->Unit(benchmark::kMillisecond);
}

class ObjectLoader
{
public:
  virtual ~ObjectLoader() {};
  virtual void load() = 0;

  lib::Object obj;
};

class RandObjectLoader : public ObjectLoader
{
public:
  const size_t nbVertices;
  const size_t nbFaces;

  RandObjectLoader(const size_t nbVertices, const size_t nbFaces)
    : ObjectLoader(), nbVertices(nbVertices), nbFaces(nbFaces) {}

  void load() {
    // check nbs
    obj = Object::randGen(nbVertices, nbFaces);
  }
};

class FileObjectLoader : public ObjectLoader
{
public:
  const std::string fileName;

  FileObjectLoader(const std::string& fileName)
    : ObjectLoader(), fileName(fileName) {}

  void load() {
      ifstream file(fileName, std::ios::binary);

      if (!file.is_open())
        throw std::runtime_error("Can't read off file");

      stringstream ss;
      ss << file.rdbuf();

      obj = Object::readOFF(ss);
  }
};

struct BM_NormalCalculationArgs {
  std::shared_ptr<ObjectLoader> objLoader;
  std::shared_ptr<NormalCalculation> calc;
};

auto BM_NormalCalculation = [](benchmark::State& state, auto Inputs)
{
  try {
    BM_NormalCalculationArgs args(Inputs);
    args.objLoader->load();
    for (auto _ : state) args.calc->calculate();
  } catch (const std::runtime_error & e) {
    state.SkipWithError(e.what());
  }
};

void registerBenchmark(std::string name, BM_NormalCalculationArgs& args)
{
  Object & obj = args.objLoader->obj;
  {
    args.calc = std::make_shared<SequentialNormalCalculation>(obj);
    auto b = benchmark::RegisterBenchmark((name+"/Sequential").c_str(), BM_NormalCalculation, args);
    benchmarkApplyArgs(b);
  }
  {
    for (int nbThreads : {2, 3, 4, 8}) {
      args.calc = std::make_shared<OpenMPNormalCalculation>(obj, nbThreads);
      auto b = benchmark::RegisterBenchmark((name+"/OpenMP/"+std::to_string(nbThreads)).c_str(), BM_NormalCalculation, args);
      benchmarkApplyArgs(b);
    }
  }
}

int main(int argc, char** argv) {

  //Random object
  for (int nbFaces : { 100'000, 500'000, 1'000'000, 2'000'000, 5'000'000, 10'000'000 }) {
    int nbVertices = nbFaces / 2;

    BM_NormalCalculationArgs args;
    args.objLoader = std::make_shared<RandObjectLoader>(nbVertices, nbFaces);

    std::string name("Rand/F:");
    name += std::to_string(nbFaces);
    name += "/V:";
    name += std::to_string(nbVertices);

    registerBenchmark(name, args);
  }

  //File object
  for (const std::string & fileName : {"sphere.off", "can1k5.off", "bouddha10k.off", "bouddha500k.off", "bouddha1m.off"}) {
    BM_NormalCalculationArgs args;
    args.objLoader = std::make_shared<FileObjectLoader>(fileName);

    std::string name("File/");
    name += fileName;

    registerBenchmark(name, args);
  }

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}
