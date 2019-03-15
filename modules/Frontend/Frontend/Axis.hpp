#pragma once
#include "RangedCurve.hpp"
#include "Vocabulary.hpp"
#include <string>

namespace Rescue
{
class Axis
{
public:
    Axis(Id id, std::string input, RangedCurve curve);

    float evaluateFor(float x) const;

    Id id;
    std::string input;
    RangedCurve curve;
};
}