#include "ActionWidget.hpp"
#include "AxisWidget.hpp"
#include "FlowLayout.hpp"
#include "ui_Action.h"

using namespace Rescue;

ActionWidget::ActionWidget(Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer, Id id, QWidget* parent)
: QWidget(parent)
, mUi(std::make_unique<Ui::Action>())
, mBus(std::move(bus))
, mActionId(id)
{
  mUi->setupUi(this);
  mUi->addAxis->setDefaultAction(mUi->actionAdd_Axis);
  // mUi->name->setText(mAction->getName().c_str());
  connect(mUi->actionAdd_Axis, &QAction::triggered, [this]() { onAddAxis(); });
  mAreaLayout = new FlowLayout();
  // mAreaLayout->addStretch();
  mUi->axisArea->setLayout(mAreaLayout);

  observer.observe([=](State const& state) { return locate(state.group, id); },
                   [this](Ptr<Action const> const& action) { updateFrom(action); });

  // connect(mUi->name, &QLineEdit::textEdited, [this](QString text) { mAction->setName(text.toStdString()); });
}

ActionWidget::~ActionWidget() = default;

void ActionWidget::onAddAxis()
{
  mBus->dispatch<Events::AddAxisTo>(createId(), mActionId);
  /*auto axis = std::make_shared<Rescue::Axis>("", Rescue::RangedCurve());
  mAction->addAxis(axis);
  addAxisWidget(axis);*/
}

AxisWidget* ActionWidget::addAxisWidget(std::shared_ptr<Rescue::Axis> const& axis)
{
  auto widget = new AxisWidget(axis, mUi->axisArea);
  mAreaLayout->addWidget(widget);
  return widget;
}

void ActionWidget::updateFrom(Ptr<Rescue::Action const> const& action)
{
  mUi->name->setText(action->name.c_str());

  auto extractId = [](auto const& axis) {return axis->id;};
  auto insert = [this](auto const& axis, auto index) {
    auto widget = new AxisWidget(std::make_shared<Axis>(createId(), "", RangedCurve{}), mUi->axisArea);
    mAreaLayout->addWidget(widget);
    return widget;
  };
  auto remove = [this](QWidget* widget) {
    delete widget;
  };
  mAxisWidgets.update(action->axisList, extractId, insert, remove);
}
