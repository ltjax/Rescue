#include "Action.hpp"

using namespace Rescue;

Axis::Axis(Id id, Id inputId, RangedCurve curve, std::string comment)
: id(id)
, inputId(inputId)
, curve(curve)
, comment(std::move(comment))
{
}

float Axis::evaluateFor(float x) const
{
  return curve.evaluateFor(x);
}

Inputs Rescue::adaptInputRanges(Inputs inputs, Group const& group)
{
  std::unordered_map<Id, std::pair<float, float>> rangeFor;

  for (auto const& action : group)
  {
    for (auto const& axis : action->axisList)
    {
      if (axis->inputId.is_nil())
        continue;

      float a = axis->curve.min();
      float b = axis->curve.max();
      // We support inverted ranges, but need them ordered now
      if (a > b)
        std::swap(a, b);

      auto found = rangeFor.find(axis->inputId);
      if (found == rangeFor.end())
      {
        rangeFor.insert(std::make_pair(axis->inputId, std::make_pair(a, b)));
      } else
      {
        auto& range = found->second;
        range.first = std::min(a, range.first);
        range.second = std::max(b, range.second);
      }
    }
  }

  for (auto& input : inputs)
  {
    auto found = rangeFor.find(input->id);
    if (found == rangeFor.end())
      continue;

    auto range = found->second;
    if (input->min == range.first && input->max == range.second)
      continue;

    auto newInput = clone(input);
    newInput->min = range.first;
    newInput->max = range.second;
    newInput->value = std::min(std::max(newInput->value, newInput->min), newInput->max);
    input = newInput;
  }
  return inputs;
}
