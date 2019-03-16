#include "InputPanel.hpp"
#include "ui_InputPanel.h"

using namespace Rescue;

InputPanel::InputPanel(Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer, Id id, QWidget* parent)
: QWidget(parent)
, mUi(std::make_unique<Ui::InputPanel>())
{
  mUi->setupUi(this);
}

InputPanel::~InputPanel() = default;


