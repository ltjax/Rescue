#pragma once
#include "Curve.hpp"

namespace Rescue
{

class RangedCurve
{
public:
    RangedCurve(Curve Curve = {}, float min = 0.f, float max = 1.f);
    ~RangedCurve();

    float evaluateFor(float Rhs) const;

    // TODO: rename to normalized curve
    Curve getCurve() const;

    void setMin(float x);
    float getMin() const;

    void setMax(float x);
    float getMax() const;

private:
    Curve mCurve;
    float mMin;
    float mMax;
};
}
