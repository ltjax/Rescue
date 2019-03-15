#include "AxisWidget.hpp"
#include "SignalBlocker.hpp"
#include "ui_Axis.h"
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
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

AxisWidget::AxisWidget(
  Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer, Id actionId, Id axisId, QWidget* parent)
: mUi(std::make_unique<Ui::Axis>())
, mBus(std::move(bus))
, mActionId(actionId)
, mAxisId(axisId)
{
  mUi->setupUi(this);
  for (std::size_t i = 0; i < typeToString.size(); ++i)
    mUi->type->addItem(typeToString[i].second, QVariant(static_cast<int>(typeToString[i].first)));

  using namespace std::placeholders;

  auto valueChanged = static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged);
  connect(mUi->m, valueChanged, directTo(&Curve::withM));
  connect(mUi->k, valueChanged, directTo(&Curve::withK));
  connect(mUi->c, valueChanged, directTo(&Curve::withC));
  connect(mUi->b, valueChanged, directTo(&Curve::withB));

  auto currentIndexChanged = static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged);
  connect(mUi->type, currentIndexChanged, [this](int current) {
    if (current < static_cast<int>(typeToString.size()))
    {
      auto type = typeToString[current].first;
      modifyCurve(std::bind(&Curve::withType, _1, type));
    }
  });

  connect(mUi->input, &QLineEdit::textEdited,
          [this](QString text) { mBus->dispatch<Events::ModifyAxisInput>(mActionId, mAxisId, text.toStdString()); });

  connect(mUi->min, valueChanged, [this](double rhs) {
    auto curve = currentCurve();
    curve.setMin(rhs);
    emitChange(curve);
  });

  connect(mUi->max, valueChanged, [this](double rhs) {
    auto curve = currentCurve();
    curve.setMax(rhs);
    emitChange(curve);
  });

  connect(mUi->remove, &QToolButton::clicked, [this](bool)
  {
    mBus->dispatch<Events::RemoveAxis>(mActionId, mAxisId);
  });

  observer.observe(
    [this](State const& state) {
      auto const& action = locate(state.group, mActionId);
      return locate(action->axisList, mAxisId);
    },
    [this](Ptr<Axis const> const& axis) { updateFrom(axis); });
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
  SignalBlocker blocker({ mUi->m, mUi->k, mUi->c, mUi->b, mUi->min, mUi->max, mUi->graphWidget, mUi->input });

  mUi->m->setValue(axis->curve.getCurve().m());
  mUi->k->setValue(axis->curve.getCurve().k());
  mUi->c->setValue(axis->curve.getCurve().c());
  mUi->b->setValue(axis->curve.getCurve().b());
  mUi->min->setValue(axis->curve.getMin());
  mUi->max->setValue(axis->curve.getMax());
  mUi->graphWidget->setRangedCurve(axis->curve);
  mUi->input->setText(axis->input.c_str());
}

Rescue::RangedCurve const& AxisWidget::currentCurve() const
{
  return mUi->graphWidget->getRangedCurve();
}
