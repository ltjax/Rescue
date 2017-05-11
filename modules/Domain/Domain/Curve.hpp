#pragma once
#include <string>

class Curve
{
public:
	enum class FunctionType
	{
		Linear,
		Polynomial,
		Logistic
	};
	Curve(FunctionType Type=FunctionType::Linear, float m=1.f, float k=1.f, float c=0.f, float b=0.f);
	float evaluateFor(float x) const;

	Curve withM(float m) const { return { mType, m, mK, mC, mB }; }
	Curve withK(float k) const { return { mType, mM, k, mC, mB }; }
	Curve withC(float c) const { return { mType, mM, mK, c, mB }; }
	Curve withB(float b) const { return { mType, mM, mK, mC, b }; }
private:
	FunctionType mType;
	float mM;
	float mK;
	float mC;
	float mB;
};
