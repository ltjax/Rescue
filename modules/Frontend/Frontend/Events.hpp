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

struct ModifyAxisCurve
{
  Id actionId;
  Id axisId;
  RangedCurve curve;
};

struct ModifyAxisInput
{
  Id actionId;
  Id axisId;
  std::string input;
};

struct NewFile
{
};
} // namespace Events
} // namespace Rescue
