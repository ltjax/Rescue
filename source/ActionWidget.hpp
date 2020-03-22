#pragma once
#include "Action.hpp"
#include "CuteDiffableList.hpp"
#include "State.hpp"
#include "Vocabulary.hpp"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QWidget>
#include <memory>
#include <ushiro/diffable_list.hpp>
#include <ushiro/event_bus.hpp>
#include <ushiro/state_observer.hpp>

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
  ActionWidget(Ptr<ushiro::event_bus> bus, ushiro::link<State> link, Id id, QWidget* parent);
  ~ActionWidget() final;

  void updateFrom(Ptr<Rescue::Action const> const& action);
  void onAddAxis();

private:
  std::unique_ptr<Ui::Action> mUi;
  Ptr<ushiro::event_bus> mBus;
  ushiro::state_observer<State> mObserver;
  Id mActionId;
  FlowLayout* mAreaLayout = nullptr;
  CuteDiffableList<AxisWidget> mAxisWidgets;
};

} // namespace Rescue
