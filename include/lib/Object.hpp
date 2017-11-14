#ifndef LIB_OBJECT_HPP
#define LIB_OBJECT_HPP

#include <deque>
#include <iostream>

#include "Face.hpp"
#include "Vector.hpp"
#include "Vertex.hpp"

namespace lib {

class Object
{
public:
  Object() = default;
  Object(const Object&) = default;
  ~Object() = default;

  inline const std::deque<Vertex>& vertices() const { return _vertices; }
  inline const std::deque<Face>& faces() const { return _faces; }

  inline const Vertex& min() const { return _min; }
  inline const Vertex& max() const { return _max; }
  inline const Vertex& centroid() const { return _centroid; }

  static Object randGen(const size_t nbVertices, const size_t nbFaces);

  static Object readOFF(std::istream&);
  void writeOFF(std::ostream&) const;

  void writeOBJ(std::ostream&, const std::deque<Vector>& vertexNormal) const;

private:
  std::deque<Vertex> _vertices;
  std::deque<Face> _faces;

  Vertex _min;
  Vertex _max;
  Vertex _centroid;
};

} // namespace lib

#endif
