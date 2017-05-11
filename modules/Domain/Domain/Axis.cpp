#include "Axis.hpp"
#include <algorithm>

namespace
{
float Saturate(float x)
{
	return std::min(1.f, std::max(0.f, x));
}
}

Axis::Axis(std::string Input, CurveType Type, float m, float k, float c, float b)
	: mInput(std::move(Input)), mType(Type), mM(m), mK(k), mC(c), mB(b)
{
}

float Axis::EvaluateFor(float x) const
{
	switch (mType)
	{
	case CurveType::Linear:
		return Saturate(mM * (x - mC) + mB);
	case CurveType::Polynomial:
		return Saturate(mM * std::pow(x - mC, mK) + mB);
	case CurveType::Logistic:
		return Saturate(mK / (1.f + std::exp(-10.f * (x + mC))) + mB);
	};
	return 0.0f;
}
