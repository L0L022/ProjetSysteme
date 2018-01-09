#include <algorithm>
#include <cstdlib>
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
  cout << "Usage : cli <OFF file> [sequential(default)|pthread|openmp] [nb "
          "threads]"
       << endl;
}

int
main(int argc, char* argv[])
{
  if (argc < 2 or argc > 4) {
    printHelp();
    return -1;
  }

  int precision = std::numeric_limits<long double>::max_digits10;
  cout.precision(precision);
  cin.precision(precision);
  cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
  cin.setf(std::ios_base::fixed, std::ios_base::floatfield);

  ifstream file(argv[1], std::ios::binary);
  file.precision(precision);
  file.setf(std::ios_base::fixed, std::ios_base::floatfield);

  if (!file.is_open()) {
    printHelp();
    cout << "Can't open OFF file.\n";
    return -1;
  }

  stringstream ss;
  ss.precision(precision);
  ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
  ss << file.rdbuf();

  Object obj = Object::readOFF(ss);

  cout << "Nombre de faces : " << obj.faces().size()
       << " Nombre de sommets : " << obj.vertices().size() << endl;

  auto method = NormalCalculation::Method::Sequential;

  if (argc == 3) {
    string strMethod(argv[2]);
    if (strMethod == "sequential")
      method = NormalCalculation::Method::Sequential;
    else if (strMethod == "pthread")
      method = NormalCalculation::Method::pThread;
    else if (strMethod == "openmp")
      method = NormalCalculation::Method::OpenMP;
    else
      cerr << "Unknown method use default instead.\n";
  }

  if (argc == 4) {
    string nbThreads(argv[3]); // check valid number
    if (method == NormalCalculation::Method::pThread)
      setenv("NB_THREADS", nbThreads.c_str(), 1);
    if (method == NormalCalculation::Method::OpenMP)
      setenv("OMP_NUM_THREADS", nbThreads.c_str(), 1);
  }

  unique_ptr<NormalCalculation> calc(NormalCalculation::factory(method, obj));
  if (calc) {
    calc->calculate();

    ofstream f("truc.obj", std::ios::binary | std::ios::out);
    f.precision(precision);
    f.setf(std::ios_base::fixed, std::ios_base::floatfield);
    obj.writeOBJ(f, calc->vertexNormal());
  } else {
    printHelp();
    cout << "Calculation not implemented.\n";
    return -1;
  }
}
