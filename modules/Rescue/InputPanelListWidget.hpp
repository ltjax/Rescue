#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
#include "event_bus.hpp"
#include "state_observer.hpp"
#include "State.hpp"
#include "InputPanel.hpp"
#include "CuteDiffableList.hpp"

namespace Rescue
{
class InputPanelListWidget : public QWidget
{
public:
  InputPanelListWidget(Ptr<ushiro::event_bus> bus, ushiro::link<State> link, QWidget* parent);

private:
  void updateFrom(Inputs const& inputs);
  Ptr<ushiro::event_bus> mBus;
  ushiro::state_observer<State> mObserver;
  QBoxLayout* mLayout = nullptr;
  CuteDiffableList<InputPanel> mWidgets;
};

}



