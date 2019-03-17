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
  mAreaLayout->setMargin(0);
  mUi->axisArea->setLayout(mAreaLayout);

  observer.observe([=](State const& state) { return locate(state.group, id); },
                   [this](Ptr<Action const> const& action) { updateFrom(action); });

  connect(mUi->name, &QLineEdit::textEdited,
          [this](QString text) { mBus->dispatch<Events::ModifyActionName>(mActionId, text.toStdString()); });

  connect(mUi->remove, &QToolButton::clicked, [this](bool)
  {
    mBus->dispatch<Events::RemoveAction>(mActionId);

  });
}

ActionWidget::~ActionWidget() = default;

void ActionWidget::onAddAxis()
{
  mBus->dispatch<Events::AddAxisTo>(createId(), mActionId);
}

void ActionWidget::updateFrom(Ptr<Rescue::Action const> const& action)
{
  mUi->name->setText(action->name.c_str());

  auto insert = [this](auto const& axis, auto index) {
    auto widget = new AxisWidget(mBus, mObserver, mActionId, axis->id, mUi->axisArea);
    mAreaLayout->insertWidget(index, widget);
    return widget;
  };

  mAxisWidgets.update(action->axisList, insert);
}
