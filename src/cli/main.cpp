#include <algorithm>
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>

#include <lib/Maths.hpp>
#include <lib/Object.hpp>
#include <lib/SequentialNormalCalculation.hpp>
#include <lib/Vector.hpp>

using namespace std;

int
main(int argc, char* argv[])
{
  ifstream fichier(argv[1], std::ios::binary);
  stringstream ss;
  ss << fichier.rdbuf();

  cout.precision(std::numeric_limits<double>::digits10 + 1);
  cin.precision(std::numeric_limits<double>::digits10 + 1);

  Object obj = Object::readOFF(ss);

  cout << "Nombre de faces : " << obj.faces().size()
       << " Nombre de sommets : " << obj.vertices().size() << endl;

  SequentialNormalCalculation calc(obj);
  calc.calculate();

  ofstream f("truc.obj", std::ios::binary | std::ios::out);
  obj.writeOBJ(f, calc.vertexNormal());
}
