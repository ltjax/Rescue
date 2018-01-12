#define CATCH_CONFIG_MAIN
#include "../modules/Frontend/Frontend/rtti_reducer.hpp"
#include <catch.hpp>

namespace
{
struct empty
{
};
struct dummy_action
{
};
}

TEST_CASE("wrapped type index works")
{
    std::unique_ptr<rtti_reducer<empty>::wrapped_base> p =
        std::make_unique<rtti_reducer<empty>::wrapped<dummy_action>>(dummy_action{});
    auto from_erased = std::type_index(typeid(*p.get()));
    auto original = std::type_index(typeid(rtti_reducer<empty>::wrapped<dummy_action>));
    REQUIRE(from_erased == original);
}

TEST_CASE("dispatch matches")
{
    rtti_reducer<empty> reducer;

    bool was_called = false;
    reducer.subscribe<dummy_action>([&](auto model, auto action) {
        static_assert(std::is_same<decltype(action), dummy_action>::value, "This needs to be the dummy_action");
        was_called = true;
        return model;
    });
    reducer({}, dummy_action{});
    REQUIRE(was_called);
}
