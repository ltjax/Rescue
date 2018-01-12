#pragma once
#include "Domain/Group.hpp"
#include "rtti_reducer.hpp"
#include <memory>

struct Model
{
    std::shared_ptr<Rescue::Group const> Group;
    std::string Filename;
};

using Reducer = rtti_reducer<Model>;
using Action = Reducer::action;