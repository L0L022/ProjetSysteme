#include <gui/Object.hpp>

#include <fstream>
#include <sstream>

using namespace gui;
using namespace std;

Object::Object(QNode* parent)
  : QComponent(parent)
{}

Object::~Object() {}

void
Object::calculateNormals(const CalculNormalMethod m)
{
  _normalCalculation.reset(lib::NormalCalculation::factory(m, _object));
  // TODO mettre du temps !
  _normalCalculation->calculate();
}

void
Object::readOFF(const QUrl& fileName)
{
  ifstream file(fileName.toLocalFile().toStdString(), std::ios::binary);
  stringstream ss;
  ss << file.rdbuf();
  _object = lib::Object::readOFF(ss);
  emit objectChanged();
}

void
Object::writeOFF(const QUrl& fileName) const
{
  stringstream ss;
  _object.writeOFF(ss);
  ofstream file(fileName.toLocalFile().toStdString(),
                std::ios::binary | std::ios::out);
  file << ss.rdbuf();
}

void
Object::writeOBJ(const QUrl& fileName) const
{
  // TODO: si on a pas fait de calcul ça ne marche pas !
  stringstream ss;
  _object.writeOBJ(ss, _normalCalculation->vertexNormal());
  ofstream file(fileName.toLocalFile().toStdString(),
                std::ios::binary | std::ios::out);
  file << ss.rdbuf();
}
