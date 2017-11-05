#include <cmath>
#include <lib/Point.hpp>
#include <lib/Vector.hpp>

Vector::Vector()
{
  // modif RR, 05/12/2006, passage 0.0 -> 0.0f
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
}

// modif RR 02/12/2006, parametre const double -> const double&
Vector::Vector(const double& X, const double& Y, const double& Z)
{
  x = X;
  y = Y;
  z = Z;
}

/*!
 * \author RR
 * \date 05/12/2006
 */
Vector::Vector(const Vector& _v)
{
  x = _v.x;
  y = _v.y;
  z = _v.z;
}

/*!
 * \author RR
 * \date 05/12/2006
 */
Vector::~Vector()
{
  // rien
}

/*!
 * \author RR
 * \date 05/12/2006
 */
Vector::Vector(const Point& _P)
{
  x = _P.x;
  y = _P.y;
  z = _P.z;
}

/*!
 * \author RR
 * \date 05/12/2006
 */
Vector::Vector(const Point& _A, const Point& _B)
{
  x = _B.x - _A.x;
  y = _B.y - _A.y;
  z = _B.z - _A.z;
}

int
Vector::operator==(const Vector& op) const
{
  return (x == op.x && y == op.y && z == op.z);
}

Vector&
Vector::operator=(const Vector& op)
{
  x = op.x;
  y = op.y;
  z = op.z;
  return *this;
}

Vector&
Vector::operator=(const Point& op)
{
  x = op.x;
  y = op.y;
  z = op.z;

  return *this;
}

Vector
Vector::operator+() const
{
  return (*this);
}

Vector
Vector::operator+(const Vector& op) const
{
  return (Vector(x + op.x, y + op.y, z + op.z));
}

/*! RR 02Juillet2008
 */
Vector
Vector::operator+(const Point& _p) const
{
  return (Vector(x + _p.x, y + _p.y, z + _p.z));
}

Vector&
Vector::operator+=(const Vector& op)
{
  x += op.x;
  y += op.y;
  z += op.z;
  return *this;
}

Vector
Vector::operator-() const
{
  return (Vector(-x, -y, -z));
}

Vector
Vector::operator-(const Vector& op) const
{
  return (Vector(x - op.x, y - op.y, z - op.z));
}

Vector&
Vector::operator-=(const Vector& op)
{
  x -= op.x;
  y -= op.y;
  z -= op.z;
  return *this;
}

Vector&
Vector::operator*=(const double op)
{
  x *= op;
  y *= op;
  z *= op;
  return *this;
}

Vector Vector::operator*(const double op) const
{
  return (Vector(x * op, y * op, z * op));
}

Vector&
Vector::operator/=(const double op)
{
  x /= op;
  y /= op;
  z /= op;
  return *this;
}

Vector
Vector::operator/(const double op) const
{
  return (Vector(x / op, y / op, z / op));
}

double Vector::operator*(const Vector& op) const
{
  return (x * op.x + y * op.y + z * op.z);
}

Vector&
Vector::operator^=(const Vector& op)
{
  Vector temp(y * op.z - z * op.y, z * op.x - x * op.z, x * op.y - y * op.x);
  x = temp.x;
  y = temp.y;
  z = temp.z;
  return *this;
}

Vector
Vector::operator^(const Vector& op) const
{
  return (
    Vector(y * op.z - z * op.y, z * op.x - x * op.z, x * op.y - y * op.x));
}

Vector
Vector::normalize(const double lg)
{
  double lgtemp;
  if (x != 0 || y != 0 || z != 0) {
    lgtemp = sqrt(x * x + y * y + z * z);
    lgtemp = lg / lgtemp;
    x *= lgtemp;
    y *= lgtemp;
    z *= lgtemp;
  }
  return (*this);
}

double
Vector::length()
{
  return (sqrt(x * x + y * y + z * z));
}

std::ostream&
operator<<(std::ostream& os, const Vector& v)
{
  return os << v.x << ' ' << v.y << ' ' << v.z;
}

std::istream&
operator>>(std::istream& is, Vector& v)
{
  return is >> v.x >> v.y >> v.z;
}
