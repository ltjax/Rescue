#pragma once
#include "Curve.hpp"
#include "Vocabulary.hpp"
#include <memory>
#include <vector>

namespace Rescue
{

struct ActionInput
{
  explicit ActionInput(Id id)
  : id(id)
  {
  }

  Id id;
  std::string name;
  float min = 0.f;
  float max = 1.f;
  float value = 0.f;
};

using Inputs = std::vector<Ptr<ActionInput const>>;

struct Axis
{
  Axis(Id id, Id inputId, RangedCurve curve);

  float evaluateFor(float x) const;

  Id id;
  Id inputId;
  RangedCurve curve;
};

struct Action
{
  explicit Action(Id id)
  : id(id)
  {
  }

  Id id;
  std::string name;
  std::vector<Ptr<Axis const>> axisList;
};

using Group = std::vector<Ptr<Action const>>;
} // namespace Rescue