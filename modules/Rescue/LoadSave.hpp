#pragma once
#include "Action.hpp"
#include <memory>
#include <pugixml.hpp>

namespace Rescue
{
namespace LoadSave
{

struct Document
{
  Inputs inputs;
  Group group;
};

Document load(Ptr<pugi::xml_document> const& document);
std::shared_ptr<pugi::xml_document> save(Group const& group);

void saveTo(std::string const& filename, Group const& group);
Document loadFrom(std::string const& filename);
}
}