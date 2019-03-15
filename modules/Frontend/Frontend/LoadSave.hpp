#pragma once
#include "Group.hpp"
#include <memory>
#include <pugixml.hpp>

namespace Rescue
{
namespace LoadSave
{

std::shared_ptr<Group> load(std::shared_ptr<pugi::xml_document> document);
std::shared_ptr<pugi::xml_document> save(std::shared_ptr<Group const> group);

void saveTo(std::string const& filename, std::shared_ptr<Group const> group);
std::shared_ptr<Group> loadFrom(std::string const& filename);
}
}