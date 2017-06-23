#pragma once
#include "Curve.hpp"
#include <string>

class Axis
{
public:
    Axis(std::string Input, Curve curve, float min, float max);
    float evaluateFor(float x) const;

    Curve const& getCurve() const;
    void setCurve(Curve curve);

    std::string const& getInput() const;
    void setInput(std::string rhs);

    void setMin(float x);
    float getMin() const;

    void setMax(float x);
    float getMax() const;

private:
    std::string mInput;
    Curve mCurve;
    float mMin = 0.f;
    float mMax = 1.f;
};