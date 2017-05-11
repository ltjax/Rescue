#pragma once
#include <string>

class Axis
{
public:
	enum class CurveType
	{
		Linear,
		Polynomial,
		Logistic
	};
	Axis(std::string Input, CurveType Type, float m, float k, float c, float b);
	float EvaluateFor(float x) const;
private:
	std::string mInput;
	CurveType mType;
	float mM;
	float mK;
	float mC;
	float mB;
};