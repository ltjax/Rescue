#pragma once

#include <tuple>

class State
{
public:
    struct TestEvent
    {

    };

    using event_list = std::tuple<TestEvent>;

    State apply(TestEvent) const
    {
        return *this;
    }
};
