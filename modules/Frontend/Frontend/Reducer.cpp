#include "Reducer.hpp"
#include "Actions.hpp"

namespace
{
template <class T> auto clone(std::shared_ptr<T> const& p)
{
    return std::make_shared<std::remove_cv_t<T>>(*p);
}
}

std::shared_ptr<rtti_reducer<Model>> createReducer()
{
    auto result = std::make_shared<rtti_reducer<Model>>();
    auto& reducer = *result;

    reducer.subscribe<ADD_ACTION>([](Model model, auto action) -> Model {
        auto NewGroup = clone(model.Group);
        NewGroup->addAction(std::make_shared<Rescue::Action>());
        return { NewGroup };
    });
    reducer.subscribe<LOAD_FROM>([](Model model, auto action) { return model; });

    return result;
}