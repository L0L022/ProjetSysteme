#include <lib/NormalCalculation.hpp>

#include <lib/OpenMPNormalCalculation.hpp>
#include <lib/PThreadNormalCalculation.hpp>
#include <lib/SequentialNormalCalculation.hpp>

using namespace lib;

NormalCalculation::NormalCalculation(const Object& object)
  : _object(object)
{
}

NormalCalculation::~NormalCalculation() {}

void
NormalCalculation::clear()
{
  _faceNormal.clear();
  _vertexNormal.clear();
}

NormalCalculation*
NormalCalculation::factory(const Method m, const Object& o)
{
  switch (m) {
    case Method::Sequential:
      return new SequentialNormalCalculation(o);
    case Method::OpenMP:
      return new OpenMPNormalCalculation(o);
    case Method::pThread:
      return new PThreadNormalCalculation(o);
    default:
      return nullptr;
      break;
  }
}
