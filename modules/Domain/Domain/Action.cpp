#include "Action.hpp"

Action::Action()
{
}

void Action::setName(std::string rhs)
{
    mName = std::move(rhs);
}

std::string const& Action::getName() const
{
    return mName;
}

PtrList<Axis> const& Action::getAxisList() const
{
    return mAxisList;
}

void Action::addAxis(std::shared_ptr<Axis> axis)
{
    mAxisList.push_back(std::move(axis));
}
