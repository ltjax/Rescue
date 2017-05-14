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

Curve const & Axis::getCurve() const
{
	return mCurve;
}

void Axis::setCurve(Curve curve)
{
	mCurve = curve;
}

std::string const & Axis::getInput() const
{
	return mInput;
}

void Axis::setInput(std::string rhs)
{
	mInput = std::move(rhs);
}
