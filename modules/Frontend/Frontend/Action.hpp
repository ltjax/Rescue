#pragma once
#include "Axis.hpp"
#include <memory>
#include <vector>
#include "Vocabulary.hpp"

namespace Rescue
{

class Action
{
public:
    explicit Action(Id id) : id(id) {}

    Id id;
    std::string name;
    std::vector<Ptr<Axis const>> axisList;
};
}