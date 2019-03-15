#pragma once

#include <boost/uuid/uuid.hpp>

namespace Rescue
{
using Id = boost::uuids::uuid;
namespace Events
{
struct AddAction
{
    Id id;
};

struct NewFile
{
};
} // namespace Events
} // namespace Rescue
