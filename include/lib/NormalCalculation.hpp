#ifndef LIB_NORMALCALCULATION_HPP
#define LIB_NORMALCALCULATION_HPP

#include "Object.hpp"
#include "Vector.hpp"
#include <deque>

namespace lib {

class NormalCalculation // permet de calculer les vecteurs normaux des faces et
                        // des sommets
{
public:
  enum class Method
  {
    Sequential,
    pThread,
    OpenMP
  };

  NormalCalculation(const Object& object);
  virtual ~NormalCalculation();

  inline const Object& object() const { return _object; }
  inline const std::deque<Vector>& faceNormal() const { return _faceNormal; }
  inline const std::deque<Vector>& vertexNormal() const
  {
    return _vertexNormal;
  }

  virtual void calculate() = 0;
  void clear();

  static NormalCalculation* factory(const Method m, const Object& o);

protected:
  const Object& _object;
  std::deque<Vector> _faceNormal;
  std::deque<Vector> _vertexNormal;
};

} // namespace lib

#endif
