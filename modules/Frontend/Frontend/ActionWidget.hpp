#pragma once
#include "Action.hpp"
#include "State.hpp"
#include "Vocabulary.hpp"
#include "event_bus.hpp"
#include "state_observer.hpp"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QWidget>
#include <memory>
#include <diffable_list.hpp>

namespace Ui
{
class Action;
}

namespace Rescue
{

class AxisWidget;
class FlowLayout;

class ActionWidget : public QWidget
{
public:
  ActionWidget(Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer, Id id, QWidget* parent);
  ~ActionWidget() final;

  void onAddAxis();
  AxisWidget* addAxisWidget(std::shared_ptr<Rescue::Axis> const& axis);

private:
  void updateFrom(Ptr<Rescue::Action const> const& action);

  std::unique_ptr<Ui::Action> mUi;
  Ptr<ushiro::event_bus> mBus;
  Id mActionId;
  FlowLayout* mAreaLayout = nullptr;
  ushiro::diffable_list<Id, AxisWidget*> mAxisWidgets;
};

} // namespace Rescue
