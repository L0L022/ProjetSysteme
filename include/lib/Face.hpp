#ifndef FACE_HPP
#define FACE_HPP

#include <iostream>

// faire un template pour size_t et point
class Face
{
public:
  size_t v0, v1, v2;

  Face();
  Face(const Face&);
  Face(const size_t v0, const size_t v1, const size_t v2);
  ~Face() = default;

  Face& operator=(const Face&);

  friend std::ostream& operator<<(std::ostream&, const Face&);
  friend std::istream& operator>>(std::istream&, Face&);
};

#endif
