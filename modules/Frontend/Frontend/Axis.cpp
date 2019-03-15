#include "Axis.hpp"
#include <algorithm>

using namespace Rescue;

Axis::Axis(Id id, std::string input, RangedCurve curve)
: input(std::move(input))
, curve(curve)
{
}

float Axis::evaluateFor(float x) const
{
    return curve.evaluateFor(x);
}
