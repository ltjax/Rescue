#pragma once

#include <QtWidgets/QWidget>
#include <ushiro/event_bus.hpp>
#include <ushiro/state_observer.hpp>
#include "State.hpp"

namespace Ui
{
class InputPanel;
}
namespace Rescue
{


class InputPanel : public QWidget
{
public:
  InputPanel(Ptr<ushiro::event_bus> bus, ushiro::link<State> link, Id id, QWidget* parent);
  ~InputPanel() final;

private:
  void updateFrom(Ptr<ActionInput const> const& input);

  std::unique_ptr<Ui::InputPanel> mUi;
  Ptr<ushiro::event_bus> mBus;
  ushiro::state_observer<State> mObserver;
};

}



