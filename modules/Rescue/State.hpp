#pragma once

#include "Action.hpp"
#include "Events.hpp"
#include "Vocabulary.hpp"
#include <tuple>
#include <unordered_map>

namespace Rescue
{

struct ActionInput
{
  Id id;
  std::string name;
  float min;
  float max;
  float value;
};

using Inputs = std::vector<Ptr<ActionInput const>>;

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
                                Events::RemoveAxis,
                                Events::RemoveAction,
                                Events::Loaded>;

  State apply(Events::NewFile const& event) const;
  State apply(Events::AddAction const& event) const;
  State apply(Events::ModifyActionName const& event) const;
  State apply(Events::AddAxisTo const& event) const;
  State apply(Events::ModifyAxisCurve const& event) const;
  State apply(Events::ModifyAxisInput const& event) const;
  State apply(Events::Loaded const& event) const;
  State apply(Events::RemoveAxis const& event) const;
  State apply(Events::RemoveAction const& event) const;

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
  Inputs inputs;
};

} // namespace Rescue
