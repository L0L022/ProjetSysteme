#ifndef VECTOR_HPP
#define VECTOR_HPP

class Point;

/*! \class Vector
 * \bug nil
 * \warning nil
 * \version 1.0
 */

class Vector {
public:
  double x;
  double y;
  double z;

  Vector(); // (0,0,0) par defaut
  Vector(const double &, const double &, const double &);
  Vector(const Point &);
  Vector(const Point &, const Point &);
  Vector(const Vector &);
  ~Vector();

  int operator==(const Vector &op) const;
  Vector &operator=(const Vector &op);
  Vector &operator=(const Point &op);
  Vector operator+() const;
  Vector operator+(const Vector &) const;
  Vector operator+(const Point &) const;

  Vector &operator+=(const Vector &op);
  Vector operator-() const;
  Vector operator-(const Vector &op) const;
  Vector &operator-=(const Vector &op);
  Vector &operator*=(const double op);      // produit par un scalaire
  Vector operator*(const double op) const;  // idem
  Vector &operator/=(const double op);      // division par un scalaire
  Vector operator/(const double op) const;  // idem
  double operator*(const Vector &op) const; // produit scalaire
  Vector &operator^=(const Vector &op);     // produit vectoriel
  Vector operator^(const Vector &op) const;

  Vector normalize(const double lg); // normalise un vecteur a la longueur lg
  double length();                   // longueur du vecteur
};

#endif
