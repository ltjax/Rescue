#pragma once
#include <vector>
#include <memory>
#include "Action.hpp"

class Group
{
public:
	Group();

	PtrList<Action> const& getActionList() const;
	void addAction(std::shared_ptr<Action> action);
private:
	PtrList<Action> mActionList;
};
