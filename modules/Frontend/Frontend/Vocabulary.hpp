#pragma once
#include <memory>

namespace Rescue
{
template <class T> using Ptr = std::shared_ptr<T>;
}
