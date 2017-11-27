#ifndef LIB_PTHREADNORMALCALCULATION_HPP
#define LIB_PTHREADNORMALCALCULATION_HPP

#include "NormalCalculation.hpp"

namespace lib {

class PThreadNormalCalculation : public NormalCalculation
{
public:
  PThreadNormalCalculation(const Object& object);
  PThreadNormalCalculation(const Object& object, const size_t threadsAmount);

  void calculate();

private:
  static void* threadCalculate(void* arg);

private:
  const size_t _threadsAmount;
};

} // namespace lib

#endif
