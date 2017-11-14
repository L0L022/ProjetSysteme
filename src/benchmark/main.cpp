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

template <class ...ExtraArgs>
static void BM_NormalCalculation(benchmark::State& state, ExtraArgs&&... extra_args) {
  Object obj = Object::randGen(10, 10);
  auto method = NormalCalculation::Method::Sequential;
  unique_ptr<NormalCalculation> calc(NormalCalculation::factory(extra_args..., obj));
  if (!calc)
    state.SkipWithError("Can't make NormalCalculation instance.");

  for (auto _ : state)
    calc->calculate();
}
BENCHMARK_CAPTURE(BM_NormalCalculation, Sequential, NormalCalculation::Method::Sequential);
// BENCHMARK_CAPTURE(BM_NormalCalculation, pThread, NormalCalculation::Method::pThread);
BENCHMARK_CAPTURE(BM_NormalCalculation, OpenMP, NormalCalculation::Method::OpenMP);

BENCHMARK_MAIN();
