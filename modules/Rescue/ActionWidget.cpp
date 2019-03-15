#include "ActionWidget.hpp"
#include "AxisWidget.hpp"
#include "FlowLayout.hpp"
#include "ui_Action.h"

using namespace Rescue;

ActionWidget::ActionWidget(Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer, Id id, QWidget* parent)
: QWidget(parent)
, mUi(std::make_unique<Ui::Action>())
, mBus(std::move(bus))
, mObserver(observer)
, mActionId(id)
{
  mUi->setupUi(this);
  mUi->addAxis->setDefaultAction(mUi->actionAdd_Axis);
  connect(mUi->actionAdd_Axis, &QAction::triggered, [this]() { onAddAxis(); });
  mAreaLayout = new FlowLayout();
  mUi->axisArea->setLayout(mAreaLayout);

  observer.observe([=](State const& state) { return locate(state.group, id); },
                   [this](Ptr<Action const> const& action) { updateFrom(action); });

  connect(mUi->name, &QLineEdit::textEdited,
          [this](QString text) { mBus->dispatch<Events::ModifyActionName>(mActionId, text.toStdString()); });
}

ActionWidget::~ActionWidget() = default;

void ActionWidget::onAddAxis()
{
  mBus->dispatch<Events::AddAxisTo>(createId(), mActionId);
}

void ActionWidget::updateFrom(Ptr<Rescue::Action const> const& action)
{
  mUi->name->setText(action->name.c_str());

  auto extractId = [](auto const& axis) { return axis->id; };
  auto insert = [this](auto const& axis, auto index) {
    auto widget = new AxisWidget(mBus, mObserver, mActionId, axis->id, mUi->axisArea);
    mAreaLayout->insertWidget(index, widget);
    widget->updateFrom(axis);
    return widget;
  };
  auto remove = [this](QWidget* widget) { delete widget; };
  mAxisWidgets.update(action->axisList, extractId, insert, remove);
}