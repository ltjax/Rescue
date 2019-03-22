#include "AxisWidget.hpp"
#include "SignalBlocker.hpp"
#include "ui_Axis.h"
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLineEdit>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <unordered_map>

using namespace Rescue;

namespace
{

std::vector<std::pair<Curve::FunctionType, QString>> const typeToString = {
  { Curve::FunctionType::Linear, "Linear" },
  { Curve::FunctionType::Polynomial, "Polynomial" },
  { Curve::FunctionType::Logistic, "Logistic" }
};
}

AxisWidget::AxisWidget(Ptr<ushiro::event_bus> bus, ushiro::link<State> link, Id actionId, Id axisId, QWidget* parent)
: mUi(std::make_unique<Ui::Axis>())
, mBus(std::move(bus))
, mObserver(std::move(link))
, mActionId(actionId)
, mAxisId(axisId)
{
  mUi->setupUi(this);
  for (const auto& each : typeToString)
    mUi->type->addItem(each.second, QVariant(static_cast<int>(each.first)));

  using namespace std::placeholders;

  mUi->input->insertItem(0, "-", boost::lexical_cast<std::string>(boost::uuids::nil_generator()()).c_str());

  auto valueChanged = qOverload<double>(&QDoubleSpinBox::valueChanged);
  connect(mUi->m, valueChanged, directTo(&Curve::withM));
  connect(mUi->k, valueChanged, directTo(&Curve::withK));
  connect(mUi->c, valueChanged, directTo(&Curve::withC));
  connect(mUi->b, valueChanged, directTo(&Curve::withB));

  auto currentIndexChanged = qOverload<int>(&QComboBox::currentIndexChanged);
  connect(mUi->type, currentIndexChanged, [this](int current) {
    if (current < static_cast<int>(typeToString.size()))
    {
      auto type = typeToString[current].first;
      modifyCurve(std::bind(&Curve::withType, _1, type));
    }
  });

  connect(mUi->input, currentIndexChanged, [this](int current) {
    auto inputId = boost::lexical_cast<Id>(mUi->input->itemData(current).toString().toStdString());
    mBus->dispatch<Events::ModifyAxisInput>(mActionId, mAxisId, inputId);
  });

  connect(mUi->min, valueChanged, [this](double rhs) {
    auto curve = currentCurve();
    curve.setMin(static_cast<float>(rhs));
    emitChange(curve);
  });

  connect(mUi->max, valueChanged, [this](double rhs) {
    auto curve = currentCurve();
    curve.setMax(static_cast<float>(rhs));
    emitChange(curve);
  });

  connect(mUi->remove, &QToolButton::clicked, [this](bool) { mBus->dispatch<Events::RemoveAxis>(mActionId, mAxisId); });

  connect(mUi->comment, &QLineEdit::textChanged, [this](QString const& text) {
    mBus->dispatch<Events::ModifyAxisComment>(mActionId, mAxisId, text.toStdString());
  });

  mObserver.observe(
    [this](State const& state) {
      auto const& action = locate(state.group, mActionId);
      return locate(action->axisList, mAxisId);
    },
    [this](Ptr<Axis const> const& axis) { updateFrom(axis); });

  mObserver.observe(
    [this](State const& state) {
      auto const& action = locate(state.group, mActionId);
      auto const& axis = locate(action->axisList, mAxisId);
      return std::tie(state.inputs, axis->inputId);
    },
    [this](Inputs const& inputs, Id inputId) { updateInputSelect(inputs, inputId); });
}

AxisWidget::~AxisWidget() = default;

void AxisWidget::emitChange(RangedCurve const& change)
{
  mBus->dispatch<Events::ModifyAxisCurve>(mActionId, mAxisId, change);
}

void AxisWidget::modifyCurve(std::function<Curve(Curve)> modifier)
{
  auto ranged = currentCurve();
  auto newCurve = modifier(ranged.getCurve());
  emitChange(newCurve);
}

void AxisWidget::updateFrom(Ptr<Rescue::Axis const> const& axis)
{
  SignalBlocker blocker(
    { mUi->m, mUi->k, mUi->c, mUi->b, mUi->min, mUi->max, mUi->graphWidget, mUi->input, mUi->comment });

  mUi->m->setValue(axis->curve.getCurve().m());
  mUi->k->setValue(axis->curve.getCurve().k());
  mUi->c->setValue(axis->curve.getCurve().c());
  mUi->b->setValue(axis->curve.getCurve().b());
  mUi->min->setValue(axis->curve.getMin());
  mUi->max->setValue(axis->curve.getMax());
  mUi->graphWidget->setRangedCurve(axis->curve);
  mUi->comment->setText(axis->comment.c_str());
}

Rescue::RangedCurve const& AxisWidget::currentCurve() const
{
  return mUi->graphWidget->getRangedCurve();
}

void AxisWidget::updateInputSelect(Inputs const& inputs, Id current)
{
  int constexpr OFFSET = 1;
  auto extractId = [](auto const& input) { return boost::lexical_cast<std::string>(input->id) + input->name; };

  auto insert = [&](Ptr<ActionInput const> const& input, int index) {
    mUi->input->insertItem(index + OFFSET, input->name.c_str(), boost::lexical_cast<std::string>(input->id).c_str());
    return index;
  };

  auto remove = [&](int, int index) { mUi->input->removeItem(index + OFFSET); };

  SignalBlocker blocker({ mUi->input });
  mInputOptions.update(inputs, extractId, insert, remove);

  auto found = std::find_if(inputs.begin(), inputs.end(), [&](auto const& input) { return input->id == current; });

  if (found != inputs.end())
  {
    mUi->input->setCurrentIndex(static_cast<int>(found - inputs.begin()) + OFFSET);
    auto x = (*found)->value;
    mUi->graphWidget->setSimulatedInput(x);
  }
  else
  {
    mUi->input->setCurrentIndex(0);
    mUi->graphWidget->setSimulatedInput({});
  }
}
