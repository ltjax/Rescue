#pragma once
#include "Axis.hpp"
#include <vector>
#include <memory>

class Action
{
public:
	Action();

private:
	std::vector<std::shared_ptr<Axis>> mAxisList;
};
