#include "State.hpp"
#include <boost/uuid/nil_generator.hpp>

using namespace Rescue;

namespace
{

} // namespace
State State::apply(Rescue::Events::NewFile const& event) const
{
  auto copy = *this;
  copy.group.clear();
  copy.inputs.clear();
  return copy;
}

State State::apply(Rescue::Events::AddAction const& event) const
{
  auto copy = *this;
  copy.group.push_back(std::make_shared<Action>(event.newId));
  return copy;
}

State State::apply(Rescue::Events::ModifyActionName const& event) const
{
  auto copy = *this;
  auto& action = locate(copy.group, event.actionId);
  auto changed = std::make_shared<Action>(*action);
  changed->name = event.name;
  action = changed;
  return copy;
}

State State::apply(Rescue::Events::AddAxisTo const& event) const
{
  auto copy = *this;
  auto const& oldAction = locate(group, event.actionId);
  auto newAction = std::make_shared<Action>(*oldAction);
  newAction->axisList.push_back(
    std::make_shared<Axis>(event.newId, boost::uuids::nil_generator{}(), RangedCurve{}, ""));
  locate(copy.group, event.actionId) = newAction;
  return copy;
}

State State::apply(Rescue::Events::ModifyAxisCurve const& event) const
{
  return modifyAxis(event.actionId, event.axisId, [&](Axis axis) {
    axis.curve = event.curve;
    return axis;
  });
}

State State::apply(Rescue::Events::ModifyAxisInput const& event) const
{
  return modifyAxis(event.actionId, event.axisId, [&](Axis axis) {
    axis.inputId = event.inputId;
    return axis;
  });
}

State State::apply(Rescue::Events::Loaded const& event) const
{
  auto copy = *this;
  copy.inputs = adaptInputRanges(event.loaded.inputs, event.loaded.group);
  copy.group = event.loaded.group;
  copy.outputs = computeOutputs(copy.inputs, copy.group);
  return copy;
}

State State::apply(Rescue::Events::RemoveAxis const& event) const
{
  auto copy = *this;
  auto& action = locate(copy.group, event.actionId);
  auto newAction = clone(action);
  auto removed = std::remove_if(newAction->axisList.begin(), newAction->axisList.end(),
                                [&](auto const& axis) { return axis->id == event.axisId; });
  newAction->axisList.erase(removed, newAction->axisList.end());
  action = newAction;
  return copy;
}

State State::apply(Events::RemoveAction const& event) const
{
  return removeObject(&State::group, event.actionId);
}

State State::apply(Events::CreateActionInput const& event) const
{
  auto copy = *this;
  auto newInput = std::make_shared<ActionInput>(event.newId);
  newInput->name = "New input";
  newInput->min = -15000.f;
  newInput->max = 15000.f;
  newInput->value = 5000.f;

  copy.inputs.push_back(newInput);
  return copy;
}

State State::apply(Events::ModifyActionInputValue const& event) const
{
  return modifyObject(&State::inputs, event.id, [&](ActionInput& input) { input.value = event.value; });
}

State State::apply(Events::ModifyActionInputName const& event) const
{
  return modifyObject(&State::inputs, event.id, [&](ActionInput& input) { input.name = event.name; });
}

State State::apply(Events::RemoveActionInput const& event) const
{
  return removeObject(&State::inputs, event.id);
}

State State::apply(Events::ModifyAxisComment const& event) const
{
  return modifyAxis(event.actionId, event.axisId, [&](Axis axis) {
    axis.comment = event.comment;
    return axis;
  });
}

Outputs Rescue::computeOutputs(Inputs const& inputs, Group const& group)
{
  std::unordered_map<Id, float> valueForInput;
  for (auto const& each : inputs)
    valueForInput.insert(std::make_pair(each->id, each->value));

  Outputs result;
  for (auto const& each : group)
  {
    float total = 1.f;
    for (auto const& axis : each->axisList)
    {
      auto found = valueForInput.find(axis->inputId);
      if (found == valueForInput.end())
      {
          total = 0.f;
          break;
      }
      float value = axis->evaluateFor(found->second);
      total *= value;
    }
    auto output = std::make_shared<Output>(each->id);
    output->value = total;
    result.push_back(std::move(output));
  }
  return result;
}