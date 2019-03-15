#pragma once

#include "Vocabulary.hpp"

namespace Rescue
{
namespace Events
{
struct AddAction
{
  Id newId;
};

struct AddAxisTo
{
  Id newId;
  Id actionId;
};

struct NewFile
{
};
} // namespace Events
} // namespace Rescue
