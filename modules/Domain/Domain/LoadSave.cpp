#include "LoadSave.hpp"

namespace
{

std::vector<std::pair<Curve::FunctionType, char const*>> const typeString = {
	{ Curve::FunctionType::Linear, "Linear" },
	{ Curve::FunctionType::Polynomial, "Polynomial" },
	{ Curve::FunctionType::Logistic, "Logistic" }
};

char const* typeToString(Curve::FunctionType type)
{
	for (auto const& each : typeString)
	{
		if (each.first == type)
			return each.second;
	}

	throw std::runtime_error("Unsupported curve type.");
}



void addCurveTo(Curve const& curve, pugi::xml_node& node)
{
	node.append_attribute("type").set_value(typeToString(curve.type()));
	node.append_attribute("m").set_value(curve.m());
	node.append_attribute("k").set_value(curve.k());
	node.append_attribute("c").set_value(curve.c());
	node.append_attribute("b").set_value(curve.b());
}
}
std::shared_ptr<Group> LoadSave::load(std::shared_ptr<pugi::xml_document> document)
{
	return std::shared_ptr<Group>();
}

std::shared_ptr<pugi::xml_document> LoadSave::save(std::shared_ptr<Group const> group)
{
	auto document = std::make_shared<pugi::xml_document>();
	auto groupNode = document->append_child("Group");

	for (auto const& action : group->getActionList())
	{
		auto actionNode = groupNode.append_child("Action");
		actionNode.append_attribute("name").set_value(action->getName().c_str());
		for (auto const& axis : action->getAxisList())
		{
			auto axisNode = actionNode.append_child("Axis");
			axisNode.append_attribute("input").set_value(axis->getInput().c_str());
			addCurveTo(axis->getCurve(), axisNode);
		}
	}

	return document;
}

void LoadSave::saveTo(std::string const & filename, std::shared_ptr<Group const> group)
{
	auto xml = save(group);
	xml->save_file(filename.c_str());
}

std::shared_ptr<Group> LoadSave::loadFrom(std::string const & filename)
{
	return std::shared_ptr<Group>();
}
