#include "Axis.hpp"
#include <algorithm>

Axis::Axis(std::string Input, Curve curve, float min, float max)
: mInput(std::move(Input)), mCurve(curve), mMin(min), mMax(max)
{
}

float Axis::evaluateFor(float x) const
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

void Axis::setMin(float x)
{
	mMin = x;
}

float Axis::getMin() const
{
	return mMin;
}

void Axis::setMax(float x)
{
	mMax = x;
}

float Axis::getMax() const
{
	return mMax;
}
