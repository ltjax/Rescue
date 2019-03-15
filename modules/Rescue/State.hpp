#pragma once

#include "Events.hpp"
#include "Action.hpp"
#include "Vocabulary.hpp"
#include <tuple>
#include <unordered_map>

namespace Rescue
{

class State
{
public:

  State() = default;
  State(State&&) = default;
  State(State const&) = default;

  State& operator=(State&&) = default;
  State& operator=(State const&) = default;

  using event_list = std::tuple<Events::AddAction,
                                Events::NewFile,
                                Events::AddAxisTo,
                                Events::ModifyAxisCurve,
                                Events::ModifyAxisInput,
                                Events::ModifyActionName,
                                Events::Loaded>;

  State apply(Events::NewFile const& event) const
  {
    auto copy = *this;
    copy.group.clear();
    return copy;
  }

  State apply(Events::AddAction const& event) const
  {
    auto copy = *this;
    copy.group.push_back(std::make_shared<Action>(event.newId));
    return copy;
  }

  State apply(Events::ModifyActionName const& event) const
  {
    auto copy = *this;
    auto& action = locate(copy.group, event.actionId);
    auto changed = std::make_shared<Action>(*action);
    changed->name = event.name;
    action=changed;
    return copy;
  }

  State apply(Events::AddAxisTo const& event) const
  {
    auto copy = *this;
    auto const& oldAction = locate(group, event.actionId);
    auto newAction = std::make_shared<Action>(*oldAction);
    newAction->axisList.push_back(std::make_shared<Axis>(event.newId, "", RangedCurve{}));
    locate(copy.group, event.actionId) = newAction;
    return copy;
  }

  State apply(Events::ModifyAxisCurve const& event) const
  {
    return modifyAxis(event.actionId, event.axisId, [&](Axis axis) {
      axis.curve = event.curve;
      return axis;
    });
  }

  State apply(Events::ModifyAxisInput const& event) const
  {
    return modifyAxis(event.actionId, event.axisId, [&](Axis axis) {
      axis.input = event.input;
      return axis;
    });
  }

  State apply(Events::Loaded const& event) const
  {
    auto copy = *this;
    copy.group = event.loaded;
    return copy;
  }

  template <typename T> State modifyAxis(Id actionId, Id axisId, T f) const
  {
    auto copy = *this;
    auto& oldAction = locate(copy.group, actionId);
    auto newAction = std::make_shared<Action>(*oldAction);
    auto& axis = locate(newAction->axisList, axisId);
    axis = std::make_shared<Axis>(f(*axis));

    oldAction = newAction;
    return copy;
  }

  Group group;
};

} // namespace Rescue
