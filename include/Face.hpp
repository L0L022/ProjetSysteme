#ifndef FACE_HPP
#define FACE_HPP

#include <iostream>

class Face {
public:
  unsigned int s0, s1, s2;

  Face();
  Face(const Face &);
  Face(const unsigned int s0, const unsigned int s1, const unsigned int s2);
  ~Face();

  Face &operator=(const Face &);

  friend std::ostream &operator<<(std::ostream &, const Face &);
  friend std::istream &operator>>(std::istream &, Face &);
};

#endif
