#pragma once
#include <vector>
#include <memory>
#include "Action.hpp"

class Group
{
public:
	Group();

	PtrList<Action> const& getActionList() const;
	std::shared_ptr<Action> addAction();
private:
	PtrList<Action> mActionList;
};
