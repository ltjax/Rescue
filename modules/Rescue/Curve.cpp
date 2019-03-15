#include "Curve.hpp"
#include <algorithm>
#include <cmath>

using namespace Rescue;

namespace
{
float Saturate(float x)
{
    return std::min(1.f, std::max(0.f, x));
}
}

Curve::Curve(FunctionType Type, float m, float k, float c, float b)
: mType(Type)
, mM(m)
, mK(k)
, mC(c)
, mB(b)
{
}

float Curve::evaluateFor(float x) const
{
    switch (mType)
    {
    case FunctionType::Linear:
        return Saturate(mM * (x - mC) + mB);
    case FunctionType::Polynomial:
        return Saturate(mM * std::pow(x - mC, mK) + mB);
    case FunctionType::Logistic:
        return Saturate(mK / (1.f + std::exp(-10.f * (x + mC))) + mB);
    };
    return 0.0f;
}

RangedCurve::RangedCurve(Curve Curve, float min, float max)
  : mCurve(Curve)
  , mMin(min)
  , mMax(max)
{
}

RangedCurve::~RangedCurve()
{
}

float RangedCurve::evaluateFor(float Rhs) const
{
    if (mMin >= mMax)
        return mCurve.evaluateFor(1.f);

    return mCurve.evaluateFor((Rhs - mMin) / (mMax - mMin));
}

Curve Rescue::RangedCurve::getCurve() const
{
    return mCurve;
}

void RangedCurve::setMin(float x)
{
    mMin = x;
}

float RangedCurve::getMin() const
{
    return mMin;
}

void RangedCurve::setMax(float x)
{
    mMax = x;
}

float RangedCurve::getMax() const
{
    return mMax;
}
