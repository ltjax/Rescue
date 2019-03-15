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
  using Group = std::vector<Identifiable<Action>>;

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
    copy.group.emplace_back(event.newId, std::make_shared<Action>());
    return copy;
  }

  State apply(Events::AddAxisTo const& event) const
  {
    auto copy = *this;
    auto oldAction = locate(group, event.actionId);
    return copy;
  }

  Group group;
};

} // namespace Rescue
