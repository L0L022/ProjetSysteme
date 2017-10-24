#include "../include/Maths.hpp"
#include "../include/Object.hpp"
#include "../include/Vector.hpp"
#include <algorithm>
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  ifstream fichier(argv[1], std::ios::binary);
  stringstream ss;
  //ss << fichier.rdbuf();

  cout.precision(std::numeric_limits<double>::digits10 + 1);
  cin.precision(std::numeric_limits<double>::digits10 + 1);

  //Object obj = Object::readOFF(ss);
  Object obj = Object::readOFF(fichier);

  cout << obj.faces().size() << ' ' << obj.points().size() << endl;

  vector<Vector> normales(obj.faces().size());
  for (int i = 0; i < normales.size(); ++i)
    normales[i] = normale(obj, i);

  vector<Point> sumPoint(obj.points().size());
  vector<int> nbPoint(obj.points().size(), 0);
  for (int i = 0; i < normales.size(); ++i) {
    const Face &f = obj.faces()[i];
    sumPoint[f.s0] += normales[i];
    ++nbPoint[f.s0];

    sumPoint[f.s1] += normales[i];
    ++nbPoint[f.s1];

    sumPoint[f.s2] += normales[i];
    ++nbPoint[f.s2];
  }

  vector<Point> normPoint(obj.points().size());
  for (int i = 0; i < normPoint.size(); ++i)
    normPoint[i] = sumPoint[i] / nbPoint[i];

    ofstream f("truc.obj", std::ios::binary | std::ios::out);
    obj.writeOBJ(f);
}
