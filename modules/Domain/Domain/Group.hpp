#pragma once
#include <vector>
#include <memory>
#include "Action.hpp"

class Group
{
public:
	Group();

private:
	std::vector<std::shared_ptr<Action>> mActionList;
};
