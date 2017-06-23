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

Curve::FunctionType typeFromString(std::string rhs)
{
    for (auto const& each : typeString)
    {
        if (each.second == rhs)
            return each.first;
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

Curve loadCurveFrom(pugi::xml_node const& node)
{
    return Curve()
        .withType(typeFromString(node.attribute("type").as_string()))
        .withM(node.attribute("m").as_float())
        .withK(node.attribute("k").as_float())
        .withC(node.attribute("c").as_float())
        .withB(node.attribute("b").as_float());
}
}
std::shared_ptr<Group> LoadSave::load(std::shared_ptr<pugi::xml_document> document)
{
    auto group = std::make_shared<Group>();
    auto groupNode = document->child("Group");

    for (auto const& actionNode : groupNode.children("Action"))
    {
        auto action = std::make_shared<Action>();
        action->setName(actionNode.attribute("name").as_string());

        for (auto const& axisNode : actionNode.children("Axis"))
        {
            auto axis = std::make_shared<Axis>(axisNode.attribute("input").as_string(), loadCurveFrom(axisNode),
                                               axisNode.attribute("min").as_float(0.f),
                                               axisNode.attribute("max").as_float(1.f));
            action->addAxis(axis);
        }
        group->addAction(action);
    }
    return group;
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
            axisNode.append_attribute("min").set_value(axis->getMin());
            axisNode.append_attribute("max").set_value(axis->getMax());
        }
    }

    return document;
}

void LoadSave::saveTo(std::string const& filename, std::shared_ptr<Group const> group)
{
    auto xml = save(group);
    xml->save_file(filename.c_str());
}

std::shared_ptr<Group> LoadSave::loadFrom(std::string const& filename)
{
    auto document = std::make_shared<pugi::xml_document>();
    auto result = document->load_file(filename.c_str());
    if (!result)
        throw std::runtime_error("Error parsing file");
    return load(document);
}
