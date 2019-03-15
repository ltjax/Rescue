#include "Action.hpp"

using namespace Rescue;

Axis::Axis(Id id, std::string input, RangedCurve curve)
  : id(id)
  , input(std::move(input))
  , curve(curve)
{
}

float Axis::evaluateFor(float x) const
{
  return curve.evaluateFor(x);
}
