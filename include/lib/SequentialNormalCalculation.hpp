#ifndef LIB_SEQUENTIALNORMALCALCULATION_HPP
#define LIB_SEQUENTIALNORMALCALCULATION_HPP

#include "NormalCalculation.hpp"

namespace lib {

class SequentialNormalCalculation : public NormalCalculation
{
public:
  SequentialNormalCalculation(const Object& object);

  void calculate();
};

} // namespace lib

#endif
