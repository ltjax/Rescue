#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
#include "event_bus.hpp"
#include "state_observer.hpp"
#include "State.hpp"

namespace Rescue
{
class InputListWidget : public QWidget
{
public:
  InputListWidget(Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer, QWidget* parent);

private:

  QBoxLayout* mLayout = nullptr;
};

}



