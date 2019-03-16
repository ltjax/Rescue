#pragma once

#include "Vocabulary.hpp"
#include "Action.hpp"

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

struct ModifyActionName
{
  Id actionId;
  std::string name;
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

struct RemoveAxis
{
  Id actionId;
  Id axisId;
};

struct RemoveAction
{
  Id actionId;
};

struct NewFile
{
};

struct Loaded
{
  Group loaded;
};

struct LoadFrom
{
  std::string filename;
};

struct SaveTo
{
  std::string filename;
};

struct CreateActionInput
{
  Id newId;
};

struct ModifyActionInputName
{
  Id id;
  std::string name;
};

struct ModifyActionInputValue
{
  Id id;
  float value;
};

struct RemoveActionInput
{
  Id id;
};

} // namespace Events
} // namespace Rescue
