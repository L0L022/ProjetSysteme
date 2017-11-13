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

#include <benchmark/Benchmark.hpp>

using namespace std;
using namespace lib;

struct BenchmarkInfo
{
  string name;
  double mean;
  double standardDeviation;
};

struct ObjectBenchmarkInfo
{
  string name;
  size_t nbVertices;
  size_t nbFaces;
  vector<BenchmarkInfo> calculationInfo;
};

ObjectBenchmarkInfo
test(const string& fileName)
{
  ifstream file(fileName, std::ios::binary);
  if (!file.is_open()) throw runtime_exception("Can't open OFF file");

  stringstream ss;
  ss << file.rdbuf();

  Object obj = Object::readOFF(ss);

  ObjectBenchmarkInfo info;
  info.name = fileName;
  info.nbVertices = obj.vertices().size();
  info.nbFaces = obj.faces().size();

  auto method = NormalCalculation::Method::Sequential;

  unique_ptr<NormalCalculation> calc(NormalCalculation::factory(method, obj));
  if (calc) {
    calc->calculate();
  }

  return info;
}

int
main(int argc, char* argv[])
{
  cout.precision(std::numeric_limits<double>::digits10 +
                 1); // ça marche avec ss ?
  cin.precision(std::numeric_limits<double>::digits10 + 1);
}
