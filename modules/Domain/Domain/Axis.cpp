#include "Axis.hpp"
#include <algorithm>

Axis::Axis(std::string Input, Curve curve)
: mInput(std::move(Input)), mCurve(curve)
{
}

float Axis::EvaluateFor(float x) const
{
	return mCurve.evaluateFor(x);
}
