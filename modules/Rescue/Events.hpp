#pragma once

#include "Vocabulary.hpp"
#include "Group.hpp"

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

} // namespace Events
} // namespace Rescue
