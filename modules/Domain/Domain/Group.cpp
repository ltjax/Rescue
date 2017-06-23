#include "Group.hpp"

using namespace Rescue;

Group::Group()
{
}

PtrList<Action> const& Group::getActionList() const
{
    return mActionList;
}

void Group::addAction(std::shared_ptr<Action> action)
{
    mActionList.push_back(action);
}
