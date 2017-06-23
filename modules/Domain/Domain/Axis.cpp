#include "Axis.hpp"
#include <algorithm>

using namespace Rescue;

Axis::Axis(std::string Input, RangedCurve curve)
: mInput(std::move(Input))
, mCurve(curve)
{
}

float Axis::evaluateFor(float x) const
{
    return mCurve.evaluateFor(x);
}

RangedCurve const& Axis::getCurve() const
{
    return mCurve;
}

RangedCurve& Axis::getCurve()
{
    return mCurve;
}

void Axis::setCurve(RangedCurve curve)
{
    mCurve = curve;
}

std::string const& Axis::getInput() const
{
    return mInput;
}

void Axis::setInput(std::string rhs)
{
    mInput = std::move(rhs);
}
