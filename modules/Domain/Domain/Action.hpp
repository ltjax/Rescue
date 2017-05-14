#pragma once
#include "Axis.hpp"
#include <vector>
#include <memory>

template <typename T>
using PtrList = std::vector<std::shared_ptr<T>>;

class Action
{
public:

	Action();

	void setName(std::string rhs);
	std::string const& getName() const;
	PtrList<Axis> const& getAxisList() const;

	std::shared_ptr<Axis> addAxis();

private:
	std::string mName;
	PtrList<Axis> mAxisList;
};
