#pragma once

#include "Action.hpp"
#include "Events.hpp"
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
                                Events::RemoveAxis,
                                Events::RemoveAction,
                                Events::Loaded,
                                Events::CreateActionInput,
                                Events::ModifyActionInputValue,
                                Events::ModifyActionInputName,
                                Events::RemoveActionInput>;

  State apply(Events::NewFile const& event) const;
  State apply(Events::AddAction const& event) const;
  State apply(Events::ModifyActionName const& event) const;
  State apply(Events::AddAxisTo const& event) const;
  State apply(Events::ModifyAxisCurve const& event) const;
  State apply(Events::ModifyAxisInput const& event) const;
  State apply(Events::Loaded const& event) const;
  State apply(Events::RemoveAxis const& event) const;
  State apply(Events::RemoveAction const& event) const;
  State apply(Events::CreateActionInput const& event) const;
  State apply(Events::ModifyActionInputValue const& event) const;
  State apply(Events::ModifyActionInputName const& event) const;
  State apply(Events::RemoveActionInput const& event) const;

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

  template <typename ListType, typename T>
  State modifyObject(ListType State::* list, Id id, T functor) const
  {
    auto copy = *this;
    auto& slot = locate(copy.*list, id);
    auto modifiable = clone(slot);
    functor(*modifiable);
    slot = modifiable;
    return copy;
  }

  template <typename ListType>
  State removeObject(ListType State::* list, Id id) const
  {
    auto copy = *this;
    auto& objects = copy.*list;
    auto removed = std::remove_if(objects.begin(), objects.end(), [&](auto const& item) {return item->id == id;});
    objects.erase(removed, objects.end());
    return copy;
  }

  Group group;
  Inputs inputs;
};

} // namespace Rescue
