#pragma once
#include "Action.hpp"
#include <memory>
#include <vector>

namespace Rescue
{
using Group = std::vector<Ptr<Action const>>;
}