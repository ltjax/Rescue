#pragma once
#include "RangedCurve.hpp"
#include <string>

namespace Rescue
{
class Axis
{
public:
    Axis(std::string Input, RangedCurve curve);
    float evaluateFor(float x) const;

    // TODO: remove this overload
    RangedCurve& getCurve();

    RangedCurve const& getCurve() const;
    void setCurve(RangedCurve curve);

    std::string const& getInput() const;
    void setInput(std::string rhs);

private:
    std::string mInput;
    RangedCurve mCurve;
};
}