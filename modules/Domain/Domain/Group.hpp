#pragma once
#include "Action.hpp"
#include <memory>
#include <vector>

namespace Rescue
{
class Group
{
public:
    Group();

    PtrList<Action> const& getActionList() const;
    void addAction(std::shared_ptr<Action> action);

private:
    PtrList<Action> mActionList;
};
}