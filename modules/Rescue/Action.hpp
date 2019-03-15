#pragma once
#include "Curve.hpp"
#include <memory>
#include <vector>
#include "Vocabulary.hpp"

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

class Action
{
public:
    explicit Action(Id id) : id(id) {}

    Id id;
    std::string name;
    std::vector<Ptr<Axis const>> axisList;
};

using Group = std::vector<Ptr<Action const>>;
}