#include "Group.hpp"

Group::Group()
{
}

PtrList<Action> const & Group::getActionList() const
{
	return mActionList;
}

std::shared_ptr<Action> Group::addAction()
{
	auto action = std::make_shared<Action>();
	mActionList.push_back(action);
	return action;
}
