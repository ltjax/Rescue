#pragma once
#include "Curve.hpp"

namespace Rescue
{

class RangedCurve
{
public:
    RangedCurve(Curve Curve, float min, float max);
    ~RangedCurve();

    float evaluateFor(float Rhs) const;

private:
    Curve mCurve;
    float mMin;
    float mMax;
};
}
