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
    Action();

    std::string name;
    std::vector<Identifiable<Axis>> axisList;
};
}