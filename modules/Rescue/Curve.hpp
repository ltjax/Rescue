#pragma once
#include <string>

namespace Rescue
{
class Curve
{
public:
    enum class FunctionType
    {
        Linear,
        Polynomial,
        Logistic
    };
    explicit Curve(FunctionType Type = FunctionType::Linear, float m = 1.f, float k = 1.f, float c = 0.f, float b = 0.f);
    float evaluateFor(float x) const;

    Curve withType(FunctionType type) const
    {
        return Curve{ type, mM, mK, mC, mB };
    }
    Curve withM(float m) const
    {
        return Curve{ mType, m, mK, mC, mB };
    }
    Curve withK(float k) const
    {
        return Curve{ mType, mM, k, mC, mB };
    }
    Curve withC(float c) const
    {
        return Curve{ mType, mM, mK, c, mB };
    }
    Curve withB(float b) const
    {
        return Curve{ mType, mM, mK, mC, b };
    }

    FunctionType type() const
    {
        return mType;
    }
    float m() const
    {
        return mM;
    }
    float k() const
    {
        return mK;
    }
    float c() const
    {
        return mC;
    }
    float b() const
    {
        return mB;
    }

private:
    FunctionType mType;
    float mM;
    float mK;
    float mC;
    float mB;
};

class RangedCurve
{
public:
  explicit RangedCurve(Curve curve = Curve{}, float min = 0.f, float max = 1.f);
  ~RangedCurve();

  float evaluateFor(float Rhs) const;

  Curve normalized() const;

  void setMin(float x);
  float min() const;

  void setMax(float x);
  float max() const;

private:
  Curve mCurve;
  float mMin;
  float mMax;
};

}