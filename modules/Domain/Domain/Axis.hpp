#pragma once
#include <string>
#include "Curve.hpp"

class Axis
{
public:
	Axis(std::string Input, Curve curve);
	float EvaluateFor(float x) const;
private:
	std::string mInput;
	Curve mCurve;
};