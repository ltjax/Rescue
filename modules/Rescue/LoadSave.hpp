#pragma once
#include "Group.hpp"
#include <memory>
#include <pugixml.hpp>

namespace Rescue
{
namespace LoadSave
{

Group load(Ptr<pugi::xml_document> const& document);
std::shared_ptr<pugi::xml_document> save(Group const& group);

void saveTo(std::string const& filename, Group const& group);
Group loadFrom(std::string const& filename);
}
}