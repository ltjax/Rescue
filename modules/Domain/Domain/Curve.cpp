#include "Curve.hpp"
#include <algorithm>

namespace
{
float Saturate(float x)
{
	return std::min(1.f, std::max(0.f, x));
}
}

Curve::Curve(FunctionType Type, float m, float k, float c, float b)
:  mType(Type), mM(m), mK(k), mC(c), mB(b)
{
}

float Curve::evaluateFor(float x) const
{
	switch (mType)
	{
	case FunctionType::Linear:
		return Saturate(mM * (x - mC) + mB);
	case FunctionType::Polynomial:
		return Saturate(mM * std::pow(x - mC, mK) + mB);
	case FunctionType::Logistic:
		return Saturate(mK / (1.f + std::exp(-10.f * (x + mC))) + mB);
	};
	return 0.0f;
}
