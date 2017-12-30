#ifndef LIB_OPENMPNORMALCALCULATION_HPP
#define LIB_OPENMPNORMALCALCULATION_HPP

#include "NormalCalculation.hpp"

#define OPENMPNORMALCALCULATION_USE_PRIVATE_VAR

namespace lib {

class OpenMPNormalCalculation : public NormalCalculation
{
public:
  OpenMPNormalCalculation(const Object& object);
  OpenMPNormalCalculation(const Object& object, const size_t threadsAmount);

  void calculate();
};

} // namespace lib

#endif
