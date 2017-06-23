#include "RangedCurve.hpp"

using namespace Rescue;

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
