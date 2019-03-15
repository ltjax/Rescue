#pragma once

#include "Domain/Group.hpp"
#include "Events.hpp"
#include "Vocabulary.hpp"
#include <tuple>
#include <unordered_map>

namespace Rescue
{

class State
{
public:
  using Id = boost::uuids::uuid;

  template <class T> struct Identifiable
  {
    Identifiable(Id id, Ptr<T const> value)
    : id(id)
    , value(std::move(value))
    {
    }

    bool operator==(Identifiable const& rhs) const
    {
      return id == rhs.id && value == rhs.value;
    }

    bool operator!=(Identifiable const& rhs) const
    {
      return !(rhs == *this);
    };

    Id id;
    Ptr<T const> value;
  };

  using Group = std::vector<Identifiable<Action>>;

  Group group;

  using event_list = std::tuple<Events::AddAction, Events::NewFile>;

  State apply(Events::NewFile const& event) const
  {
    auto copy = *this;
    copy.group.clear();
    return *this;
  }

  State apply(Events::AddAction const& event) const
  {
    auto copy = *this;
    copy.group.emplace_back(event.id, std::make_shared<Action>());
    return *this;
  }
};

} // namespace Rescue
