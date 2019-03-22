#include "LoadSave.hpp"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <clocale>

using namespace Rescue;

namespace
{

class IdCompressor
{
public:
  explicit IdCompressor(std::size_t startIndex = 0)
  : mIndex(0)
  {

  }

  std::size_t operator[](Id id)
  {
    auto found = mLookup.find(id);
    if (found == mLookup.end())
    {
      auto result = mIndex++;
      mLookup.insert(std::make_pair(id, result));
      return result;
    }
    return found->second;
  }

private:
  std::size_t mIndex;
  std::unordered_map<Id, std::size_t> mLookup;
};

class IdDecompressor
{
public:
  Id operator[](std::size_t rhs)
  {
    auto found = mLookup.find(rhs);
    if (found == mLookup.end())
    {
      auto result = createId();
      mLookup.insert(std::make_pair(rhs, result));
      return result;
    }
    return found->second;
  }
private:
  std::unordered_map<std::size_t, Id> mLookup;
};

std::vector<std::pair<Curve::FunctionType, char const*>> const typeString = {
  { Curve::FunctionType::Linear, "Linear" },
  { Curve::FunctionType::Polynomial, "Polynomial" },
  { Curve::FunctionType::Logistic, "Logistic" }
};

std::tuple<int, int, int> getVersion(pugi::xml_node const& group)
{
  auto found = std::find_if(group.attributes_begin(), group.attributes_end(),
                            [](pugi::xml_attribute const& x) { return std::strcmp(x.name(), "version") == 0; });

  if (found == group.attributes_end())
  {
    return std::make_tuple(0, 1, 0);
  }

  auto value = std::string(found->value());

  std::vector<std::string> parts;
  boost::algorithm::split(parts, value, boost::algorithm::is_any_of("."), boost::algorithm::token_compress_on);
  if (parts.size() < 2 || parts.size() > 3)
    throw std::runtime_error("Invalid version string: " + value);

  auto major = boost::lexical_cast<unsigned int>(parts[0]);
  auto minor = boost::lexical_cast<unsigned int>(parts[1]);
  auto patch = parts.size() != 2 ? boost::lexical_cast<unsigned int>(parts[2]) : 0;
  return std::make_tuple(major, minor, patch);
}

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
  addCurveTo(rangedCurve.normalized(), node);
  node.append_attribute("min").set_value(rangedCurve.min());
  node.append_attribute("max").set_value(rangedCurve.max());
}

Curve loadCurveFrom(pugi::xml_node const& node)
{
  auto curve = Curve()
                 .withType(typeFromString(node.attribute("type").as_string()))
                 .withM(node.attribute("m").as_float())
                 .withK(node.attribute("k").as_float())
                 .withC(node.attribute("c").as_float())
                 .withB(node.attribute("b").as_float());
  return curve;
}

RangedCurve loadRangedCurveFrom(pugi::xml_node const& node)
{
  return RangedCurve{ loadCurveFrom(node), node.attribute("min").as_float(0.f), node.attribute("max").as_float(1.f) };
}

LoadSave::Document loadWithLocaleFixed(std::shared_ptr<pugi::xml_document> const& document)
{
  Inputs inputs;
  Group group;

  auto groupNode = document->child("Group");

  if (getVersion(groupNode) <= std::make_tuple(0, 1, 0))
  {
    std::unordered_map<std::string, Ptr<ActionInput const>> nameToInput;

    for (auto const& actionNode : groupNode.children("Action"))
    {
      auto action = std::make_shared<Action>(createId());
      action->name = actionNode.attribute("name").as_string();

      for (auto const& axisNode : actionNode.children("Axis"))
      {
        auto inputName = axisNode.attribute("input").as_string();
        auto& input = nameToInput[inputName];
        if (input == nullptr)
        {
          auto newInput = std::make_shared<ActionInput>(createId());
          newInput->name = inputName;
          input = newInput;
          inputs.push_back(input);
        }
        auto axis = std::make_shared<Axis>(createId(), input->id, loadRangedCurveFrom(axisNode), "");
        action->axisList.push_back(axis);
      }
      group.push_back(action);
    }
  }
  else
  {
    IdDecompressor inputIds;

    for (auto const& inputNode : groupNode.children("Input"))
    {
      auto id = inputIds[inputNode.attribute("id").as_uint()];
      auto input = std::make_shared<ActionInput>(id);
      input->name = inputNode.attribute("name").as_string("");
      input->value = inputNode.attribute("value").as_float(0.f);
      inputs.push_back(input);
    }

    for (auto const& actionNode : groupNode.children("Action"))
    {
      auto action = std::make_shared<Action>(createId());
      action->name = actionNode.attribute("name").as_string();

      for (auto const& axisNode : actionNode.children("Axis"))
      {
        auto inputId = inputIds[axisNode.attribute("inputId").as_uint()];
        auto comment = axisNode.attribute("comment").as_string("");
        auto axis = std::make_shared<Axis>(createId(), inputId, loadRangedCurveFrom(axisNode), comment);
        action->axisList.push_back(axis);
      }
      group.push_back(action);
    }
  }

  return { std::move(inputs), std::move(group) };
}

} // namespace

LoadSave::Document LoadSave::load(std::shared_ptr<pugi::xml_document> const& document)
{
  auto currentLocale = std::setlocale(LC_ALL, nullptr);
  std::setlocale(LC_ALL, "C");

  try
  {
    auto result = loadWithLocaleFixed(document);
    std::setlocale(LC_ALL, currentLocale);
    return result;
  }
  catch (...)
  {
    std::setlocale(LC_ALL, currentLocale);
    throw;
  }
}

std::shared_ptr<pugi::xml_document> LoadSave::save(Document const& source)
{
  auto document = std::make_shared<pugi::xml_document>();
  auto groupNode = document->append_child("Group");
  groupNode.append_attribute("version").set_value("0.2");
  IdCompressor inputIds;

  for (auto const& input : source.inputs)
  {
    auto inputNode = groupNode.append_child("Input");
    inputNode.append_attribute("id").set_value(inputIds[input->id]);
    inputNode.append_attribute("name").set_value(input->name.c_str());
    inputNode.append_attribute("value").set_value(input->value);
  }

  for (auto const& action : source.group)
  {
    auto actionNode = groupNode.append_child("Action");
    actionNode.append_attribute("name").set_value(action->name.c_str());
    for (auto const& axis : action->axisList)
    {
      auto axisNode = actionNode.append_child("Axis");
      axisNode.append_attribute("inputId").set_value(inputIds[axis->inputId]);
      addRangedCurveTo(axis->curve, axisNode);
      axisNode.append_attribute("comment").set_value(axis->comment.c_str());
    }
  }

  return document;
}

void LoadSave::saveTo(std::string const& filename, Document const& source)
{
  auto xml = save(source);
  xml->save_file(filename.c_str());
}

LoadSave::Document LoadSave::loadFrom(std::string const& filename)
{
  auto document = std::make_shared<pugi::xml_document>();
  auto result = document->load_file(filename.c_str());
  if (!result)
    throw std::runtime_error("Error parsing file");
  return load(document);
}
