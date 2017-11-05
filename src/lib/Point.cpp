#include <iostream>
#include <lib/Point.hpp>
#include <lib/Vector.hpp>

using namespace std;

/*!
 *   \brief constructeur sans paramètres
 *   \todo none
 *   \note RR: modif 19/12/2006 : =0 -> =0.0 pour les doubles
 */
Point::Point()
  : x(0.0)
  , y(0.0)
  , z(0.0)
{}

/*!
 *   \brief constructeur à partir de 3 réels (double)
 *   \todo none
 */
Point::Point(const double x, const double y, const double z)
  : x(x)
  , y(y)
  , z(z)
{}

/*!
 *   \brief constructeur de copie
 *   \todo none
 */
Point::Point(const Point& p)
  : x(p.x)
  , y(p.y)
  , z(p.z)
{}

/*!
 *   \brief constructeur de copie de vecteur3
 *   \todo none
 */
Point::Point(const Vector& v)
  : x(v.x)
  , y(v.y)
  , z(v.z)
{}

/*!
 *   \brief destructeur
 *   \todo none
 */
Point::~Point() {}

/*!
 *   \brief conditionnelle ==
 *   \todo none
 */
bool
Point::operator==(const Point& p) const
{
  return x == p.x && y == p.y && z == p.z;
}

/*!
 *   \brief affectation
 *   \todo none
 */
Point&
Point::operator=(const Point& p)
{
  x = p.x;
  y = p.y;
  z = p.z;

  return *this;
}

/*!
 *   \brief affectation
 *   \todo none
 */
Point&
Point::operator=(const Vector& v)
{
  x = v.x;
  y = v.y;
  z = v.z;

  return *this;
}

/*!
 *   \brief addition point+vecteur
 *   \todo none
 */
Point
Point::operator+(const Vector& v) const
{
  return Point(x + v.x, y + v.y, z + v.z);
}

/*!
 *   \brief addition point+point
 *   \todo none
 */
Point
Point::operator+(const Point& p) const
{
  return Point(x + p.x, y + p.y, z + p.z);
}

/*!
 *   \brief combinaison affectation/addition avec un vecteur (translation)
 *   \todo none
 */
Point&
Point::operator+=(const Vector& v)
{
  x += v.x;
  y += v.y;
  z += v.z;

  return *this;
}

/*!
 *   \brief soustraction de points
 *   \todo none
 */
Point
Point::operator-(const Point& p) const
{
  return Point(x - p.x, y - p.y, z - p.z);
}

/*!
 *   \brief combinaison affectation/multiplication par un scalaire
 *   \todo none
 */
Point&
Point::operator*=(const double n)
{
  x *= n;
  y *= n;
  z *= n;

  return *this;
}

/*!
 *   \brief multiplication par un scalaire (mise à l'échelle)
 *   \todo none
 */
Point Point::operator*(const double n) const
{
  return Point(x * n, y * n, z * n);
}

/*!
 *   \brief combinaison affectation/division par un scalaire
 *   \todo none
 *   \note RR: modif 19/12/2006 : test sur op!=0
 */
Point&
Point::operator/=(const double n)
{
  if (n == 0.0) throw runtime_error("Division by zero");

  x /= n;
  y /= n;
  z /= n;

  return *this;
}

/*!
 *   \brief division par un scalaire
 *   \todo none
 */
Point
Point::operator/(const double n) const
{
  if (n == 0.0) throw runtime_error("Division by zero");

  return Point(x / n, y / n, z / n);
}

/*!
 *   \brief multiplication par un point (produit scalaire)
 *   \todo none
 */
Point Point::operator*(const Point& p) const
{
  return Point(x * p.x, y * p.y, z * p.z);
}

/*!
 *   \brief combinaison affectation/multiplication par un point
 *   \todo none
 */
Point&
Point::operator*=(const Point& p)
{
  x *= p.x;
  y *= p.y;
  z *= p.z;

  return *this;
}

/*!
 *   \brief division par un point
 *   \todo none
 */
Point
Point::operator/(const Point& p) const
{
  if (p.x == 0.0 || p.y == 0.0 || p.z == 0.0)
    throw runtime_error("Division by zero");

  return Point(x / p.x, y / p.y, z / p.z);
}

/*!
 *   \brief combinaison affectation/division par un point
 *   \todo none
 */
Point&
Point::operator/=(const Point& p)
{
  if (p.x == 0.0 || p.y == 0.0 || p.z == 0.0)
    throw runtime_error("Division by zero");

  x /= p.x;
  y /= p.y;
  z /= p.z;

  return *this;
}

/*!
 *   \brief ecriture dans un flux ostream "(x, y, z)"
 *   \todo none
 */
ostream&
operator<<(ostream& _os, const Point& _p)
{
  return _os << _p.x << ' ' << _p.y << ' ' << _p.z;
}

/*!
 *   \brief lecture dans un flux istream "Entrez x:\n ...y:\n ...z:"
 *   \todo none
 */
istream&
operator>>(istream& p, Point& op)
{
  return p >> op.x >> op.y >> op.z;
}
