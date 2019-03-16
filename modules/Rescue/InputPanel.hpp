#pragma once

#include <QtWidgets/QWidget>
#include "event_bus.hpp"
#include "state_observer.hpp"
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
  InputPanel(Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer, Id id, QWidget* parent);
  ~InputPanel() final;

private:
  std::unique_ptr<Ui::InputPanel> mUi;
};

}



