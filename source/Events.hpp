#pragma once

#include "Vocabulary.hpp"
#include "Action.hpp"
#include "LoadSave.hpp"

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
  Id inputId;
};

struct ModifyAxisComment
{
  Id actionId;
  Id axisId;
  std::string comment;
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
  LoadSave::Document loaded;
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
