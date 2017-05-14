#pragma once
#include <string>
#include "Curve.hpp"

class Axis
{
public:
	Axis(std::string Input, Curve curve);
	float EvaluateFor(float x) const;
	Curve const& getCurve() const;
	void setCurve(Curve curve);
	std::string const& getInput() const;
	void setInput(std::string rhs);
private:
	std::string mInput;
	Curve mCurve;
};