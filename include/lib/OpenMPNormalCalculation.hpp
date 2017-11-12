#ifndef LIB_OPENMPNORMALCALCULATION_HPP
#define LIB_OPENMPNORMALCALCULATION_HPP

#include "NormalCalculation.hpp"

namespace lib {

class OpenMPNormalCalculation : public NormalCalculation
{
public:
  OpenMPNormalCalculation(const Object& object);

  void calculate();
};

} // namespace lib

#endif
