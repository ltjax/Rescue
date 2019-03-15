#include "LoadSave.hpp"

using namespace Rescue;

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

void addRangedCurveTo(RangedCurve const& rangedCurve, pugi::xml_node& node)
{
    addCurveTo(rangedCurve.getCurve(), node);
    node.append_attribute("min").set_value(rangedCurve.getMin());
    node.append_attribute("max").set_value(rangedCurve.getMax());
}

Curve loadCurveFrom(pugi::xml_node const& node)
{
    auto curve =
     Curve()
        .withType(typeFromString(node.attribute("type").as_string()))
        .withM(node.attribute("m").as_float())
        .withK(node.attribute("k").as_float())
        .withC(node.attribute("c").as_float())
        .withB(node.attribute("b").as_float());
    return curve;
}

RangedCurve loadRangedCurveFrom(pugi::xml_node const& node)
{
    return { loadCurveFrom(node), node.attribute("min").as_float(0.f), node.attribute("max").as_float(1.f) };
}
}

Group LoadSave::load(std::shared_ptr<pugi::xml_document> const& document)
{
    auto currentLocale = std::setlocale(LC_ALL, nullptr);
    std::setlocale(LC_ALL, "C");
    Group group;
    auto groupNode = document->child("Group");

    for (auto const& actionNode : groupNode.children("Action"))
    {
        auto action = std::make_shared<Action>(createId());
        action->name = actionNode.attribute("name").as_string();

        for (auto const& axisNode : actionNode.children("Axis"))
        {
            auto axis = std::make_shared<Axis>(createId(), axisNode.attribute("input").as_string(), loadRangedCurveFrom(axisNode));
            action->axisList.push_back(axis);
        }
        group.push_back(action);
    }
    std::setlocale(LC_ALL, currentLocale);
    return group;
}

std::shared_ptr<pugi::xml_document> LoadSave::save(Group const& group)
{
    auto document = std::make_shared<pugi::xml_document>();
    auto groupNode = document->append_child("Group");

    for (auto const& action : group)
    {
        auto actionNode = groupNode.append_child("Action");
        actionNode.append_attribute("name").set_value(action->name.c_str());
        for (auto const& axis : action->axisList)
        {
            auto axisNode = actionNode.append_child("Axis");
            axisNode.append_attribute("input").set_value(axis->input.c_str());
            addRangedCurveTo(axis->curve, axisNode);
        }
    }

    return document;
}

void LoadSave::saveTo(std::string const& filename, Group const& group)
{
    auto xml = save(group);
    xml->save_file(filename.c_str());
}

Group LoadSave::loadFrom(std::string const& filename)
{
    auto document = std::make_shared<pugi::xml_document>();
    auto result = document->load_file(filename.c_str());
    if (!result)
        throw std::runtime_error("Error parsing file");
    return load(document);
}