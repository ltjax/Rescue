#include "Action.hpp"

using namespace Rescue;

Axis::Axis(Id id, Id inputId, RangedCurve curve)
: id(id)
, inputId(std::move(inputId))
, curve(curve)
{
}

float Axis::evaluateFor(float x) const
{
  return curve.evaluateFor(x);
}
