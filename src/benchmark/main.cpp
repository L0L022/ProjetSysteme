#include <algorithm>
#include <fstream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <lib/Maths.hpp>
#include <lib/NormalCalculation.hpp>
#include <lib/Object.hpp>
#include <lib/Vector.hpp>

#include <benchmark/benchmark.h>

using namespace std;
using namespace lib;

struct ArgsFileNormalCalculation
{
  std::string fileName;
  lib::NormalCalculation::Method method;
};

template<class... ExtraArgs>
static void
BM_FileNormalCalculation(benchmark::State& state, ExtraArgs&&... extra_args)
{
  ArgsFileNormalCalculation args = { extra_args... };

  ifstream file(args.fileName, std::ios::binary);
  if (!file.is_open()) {
    state.SkipWithError("Can't open OFF file.");
    return;
  }
  stringstream ss;
  ss << file.rdbuf();

  Object obj = Object::readOFF(ss);

  unique_ptr<NormalCalculation> calc(
    NormalCalculation::factory(args.method, obj));
  if (!calc) state.SkipWithError("Can't make NormalCalculation instance.");

  for (auto _ : state) calc->calculate();
}

// BENCHMARK_CAPTURE(BM_FileNormalCalculation,
//                   Sequential,
//                   "bouddha1m.off",
//                   NormalCalculation::Method::Sequential);

template<class... ExtraArgs>
static void
BM_NormalCalculation(benchmark::State& state, ExtraArgs&&... extra_args)
{
  Object obj = Object::randGen(state.range(0), state.range(0));
  auto method = NormalCalculation::Method::Sequential;
  unique_ptr<NormalCalculation> calc(
    NormalCalculation::factory(extra_args..., obj));
  if (!calc) state.SkipWithError("Can't make NormalCalculation instance.");

  for (auto _ : state) calc->calculate();
}

static void
ObjectRandArgs(benchmark::internal::Benchmark* b)
{
  b->Unit(benchmark::kMillisecond);
  // for (int i = 50; i <= 1'000'000; i += 199990) b->Args({ i / 2, i });
  // for (int j = 25; j <= 500'000; j += 99995) b->Args({ i, j });
  for (int i :
       { 0, 100'000, 500'000, 1'000'000, 2'000'000, 5'000'000, 10'000'000 })
    b->Args({ i / 2, i });
}

BENCHMARK_CAPTURE(BM_NormalCalculation,
                  Sequential,
                  NormalCalculation::Method::Sequential)
  ->Apply(ObjectRandArgs);
// BENCHMARK_CAPTURE(BM_NormalCalculation, pThread,
// NormalCalculation::Method::pThread);
BENCHMARK_CAPTURE(BM_NormalCalculation,
                  OpenMP,
                  NormalCalculation::Method::OpenMP)
  ->Apply(ObjectRandArgs);

BENCHMARK_MAIN();
