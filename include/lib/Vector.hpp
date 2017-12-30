#ifndef LIB_VECTOR_HPP
#define LIB_VECTOR_HPP

#include <iostream>

namespace lib {

class Vertex;

class Vector
{
public:
  double x;
  double y;
  double z;

  Vector(); // (0,0,0) par defaut
  Vector(const double&, const double&, const double&);
  Vector(const Vertex&);
  Vector(const Vertex&, const Vertex&);
  Vector(const Vector&);
  ~Vector();

  bool operator==(const Vector& op) const;
  Vector& operator=(const Vector& op);
  Vector& operator=(const Vertex& op);
  Vector operator+() const;
  Vector operator+(const Vector&) const;
  Vector operator+(const Vertex&) const;

  Vector& operator+=(const Vector& op);
  Vector operator-() const;
  Vector operator-(const Vector& op) const;
  Vector& operator-=(const Vector& op);
  Vector& operator*=(const double op);      // produit par un scalaire
  Vector operator*(const double op) const;  // idem
  Vector& operator/=(const double op);      // division par un scalaire
  Vector operator/(const double op) const;  // idem
  double operator*(const Vector& op) const; // produit scalaire
  Vector& operator^=(const Vector& op);     // produit vectoriel
  Vector operator^(const Vector& op) const;

  Vector normalize(const double lg); // normalise un vecteur a la longueur lg
  double length();                   // longueur du vecteur

  friend std::ostream& operator<<(std::ostream&, const Vector&);
  friend std::istream& operator>>(std::istream&, Vector&);
};

} // namespace lib

#endif
