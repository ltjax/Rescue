#pragma once
#include "Axis.hpp"
#include <memory>
#include <vector>

namespace Rescue
{
template <typename T> using PtrList = std::vector<std::shared_ptr<T>>;

class Action
{
public:
    Action();

    void setName(std::string rhs);
    std::string const& getName() const;
    PtrList<Axis> const& getAxisList() const;

    void addAxis(std::shared_ptr<Axis> axis);

private:
    std::string mName;
    PtrList<Axis> mAxisList;
};
}