#include <iostream>
#include <string>
#include <random>
#include <limits>

#include <lib/Maths.hpp>
#include <lib/Object.hpp>

using namespace lib;

Object
Object::randGen(const size_t nbVertices, const size_t nbFaces)
{
  Object obj;
  std::default_random_engine gen;
  std::uniform_real_distribution<double> realDist(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
  std::uniform_int_distribution<size_t> intDist(0, nbVertices - 1);

  for (size_t i = 0; i < nbVertices; ++i)
    obj._vertices.emplace_back(realDist(gen), realDist(gen), realDist(gen));

  for (size_t i = 0; i < nbFaces; i++)
    obj._faces.emplace_back(intDist(gen), intDist(gen), intDist(gen));

  return obj;
}

Object
Object::readOFF(std::istream& is)
{
  std::string magic;
  is >> magic;

  if (magic != "OFF") throw std::runtime_error("Wrong magic number");

  Object obj;
  size_t arraySize;
  is >> arraySize;
  obj._vertices.resize(arraySize);
  is >> arraySize;
  obj._faces.resize(arraySize);
  is >> arraySize;

  for (Vertex& p : obj._vertices) {
    is >> p;

    if (obj._min.x > p.x) obj._min.x = p.x;
    if (obj._min.y > p.y) obj._min.y = p.y;
    if (obj._min.z > p.z) obj._min.z = p.z;

    if (obj._max.x < p.x) obj._max.x = p.x;
    if (obj._max.y < p.y) obj._max.y = p.y;
    if (obj._max.z < p.z) obj._max.z = p.z;
  }

  for (Face& f : obj._faces) is >> f;

  obj._centroid = (obj._max + obj._min) / 2.0;

  return obj;
}

void
Object::writeOFF(std::ostream& os) const
{
  os << "OFF\n" << _vertices.size() << ' ' << _faces.size() << ' ' << 0 << '\n';
  for (const Vertex& p : _vertices) os << p << '\n';
  for (const Face& f : _faces) os << f << '\n';
}

void
Object::writeOBJ(std::ostream& os, const std::deque<Vector>& vertexNormal) const
{
  if (_vertices.size() != vertexNormal.size())
    throw std::runtime_error(
      "La taille de vertices et vertexNormal ne correspondes pas");

  for (const Vertex& p : _vertices) os << "v " << p << '\n';
  for (const Vector& v : vertexNormal) os << "vn " << v << '\n';
  for (const Face& f : _faces)
    os << "f " << f.v0 + 1 << "//" << f.v0 + 1 << ' ' << f.v1 + 1 << "//"
       << f.v1 + 1 << ' ' << f.v2 + 1 << "//" << f.v2 + 1 << '\n';
}
