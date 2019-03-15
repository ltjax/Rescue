#pragma once

#include "Domain/Group.hpp"
#include <tuple>

namespace Rescue
{

class State
{
public:
    Rescue::Group group;

    struct TestEvent
    {
    };

    using event_list = std::tuple<TestEvent>;

    State apply(TestEvent) const
    {
        return *this;
    }
};

} // namespace Rescue
