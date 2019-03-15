#pragma once

#include "Domain/Group.hpp"
#include "Vocabulary.hpp"
#include "Events.hpp"
#include <tuple>
#include <unordered_map>
#include <boost/uuid/uuid.hpp>
#include <boost/functional/hash.hpp>

namespace Rescue
{

class State
{
public:
    using Id = boost::uuids::uuid;
    template <class T>
    using IdMap = std::unordered_map<Id, T, boost::hash<boost::uuids::uuid>>;

    template <class T>
    struct Identifiable
    {
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
