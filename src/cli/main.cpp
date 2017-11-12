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

using namespace std;
using namespace lib;

void
printHelp()
{
  cout << "Usage : cli <OFF file> [sequential(default)|pthread|openmp]" << endl;
}

int
main(int argc, char* argv[])
{
  if (argc != 2 and argc != 3) {
    printHelp();
    return -1;
  }

  ifstream file(argv[1], std::ios::binary);

  if (!file.is_open()) {
    printHelp();
    cout << "Can't open OFF file.\n";
    return -1;
  }

  stringstream ss;
  ss << file.rdbuf();

  cout.precision(std::numeric_limits<double>::digits10 + 1);
  cin.precision(std::numeric_limits<double>::digits10 + 1);

  Object obj = Object::readOFF(ss);

  cout << "Nombre de faces : " << obj.faces().size()
       << " Nombre de sommets : " << obj.vertices().size() << endl;

  auto method = NormalCalculation::Method::Sequential;

  if (argc == 3) {
    string strMethod(argv[2]);
    if (strMethod == "sequential")
      method = NormalCalculation::Method::Sequential;
    if (strMethod == "pthread") method = NormalCalculation::Method::pThread;
    if (strMethod == "openmp") method = NormalCalculation::Method::OpenMP;
  }

  unique_ptr<NormalCalculation> calc(NormalCalculation::factory(method, obj));
  if (calc) {
    calc->calculate();

    ofstream f("truc.obj", std::ios::binary | std::ios::out);
    obj.writeOBJ(f, calc->vertexNormal());
  } else {
    printHelp();
    cout << "Calculation not implemented.\n";
    return -1;
  }
}
