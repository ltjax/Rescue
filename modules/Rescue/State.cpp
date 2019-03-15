#include "State.hpp"

using namespace Rescue;

namespace
{
template <typename T> Ptr<T> clone(Ptr<T const> const& p)
{
  return std::make_shared<T>(*p);
}

} // namespace
State State::apply(Rescue::Events::NewFile const& event) const
{
  auto copy = *this;
  copy.group.clear();
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
  newAction->axisList.push_back(std::make_shared<Axis>(event.newId, "", RangedCurve{}));
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
    axis.input = event.input;
    return axis;
  });
}

State State::apply(Rescue::Events::Loaded const& event) const
{
  auto copy = *this;
  copy.group = event.loaded;
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
  auto copy = *this;
  auto removed = std::remove_if(copy.group.begin(), copy.group.end(), [&](auto const& action) {return action->id == event.actionId;});
  copy.group.erase(removed, copy.group.end());
  return copy;
}
