#include "Action.hpp"

Action::Action()
{
}

void Action::setName(std::string rhs)
{
	mName = std::move(rhs);
}

std::string const & Action::getName() const
{
	return mName;
}

PtrList<Axis> const & Action::getAxisList() const
{
	return mAxisList;
}

std::shared_ptr<Axis> Action::addAxis()
{
	mAxisList.push_back(std::make_shared<Axis>("", Curve{}));
	return mAxisList.back();
}
