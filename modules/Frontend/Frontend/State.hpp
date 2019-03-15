#pragma once

#include "Group.hpp"
#include "Events.hpp"
#include "Vocabulary.hpp"
#include <tuple>
#include <unordered_map>

namespace Rescue
{

class State
{
public:
  using Group = std::vector<Ptr<Action const>>;

  State() = default;
  State(State&&) = default;
  State(State const&) = default;

  State& operator=(State&&) = default;
  State& operator=(State const&) = default;

  using event_list = std::tuple<Events::AddAction, Events::NewFile, Events::AddAxisTo>;

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

  State apply(Events::AddAxisTo const& event) const
  {
    auto copy = *this;
    auto oldAction = locate(group, event.actionId);
    auto newAction = std::make_shared<Action>(*oldAction);
    newAction->axisList.push_back(std::make_shared<Axis>(event.newId, "", RangedCurve{}));
    locate(copy.group, event.actionId) = newAction;
    return copy;
  }

  Group group;
};

} // namespace Rescue
