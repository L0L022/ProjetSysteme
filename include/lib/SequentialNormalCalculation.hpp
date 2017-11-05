#ifndef SEQUENTIALNORMALCALCULATION_HPP
#define SEQUENTIALNORMALCALCULATION_HPP

#include "NormalCalculation.hpp"

class SequentialNormalCalculation : public NormalCalculation
{
public:
  SequentialNormalCalculation(const Object& object);

  void calculate();
};

#endif
