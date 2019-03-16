#include "InputPanel.hpp"
#include "SignalBlocker.hpp"
#include "ui_InputPanel.h"

using namespace Rescue;

InputPanel::InputPanel(Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer, Id id, QWidget* parent)
: QWidget(parent)
, mUi(std::make_unique<Ui::InputPanel>())
, mBus(std::move(bus))
{
  mUi->setupUi(this);

  observer.observe([id](State const& state) { return locate(state.inputs, id); },
                   [this](auto const& input) { updateFrom(input); });

  connect(mUi->spinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
          [this, id](double value) { mBus->dispatch<Events::ModifyActionInputValue>(id, static_cast<float>(value)); });

  connect(mUi->slider, &QSlider::valueChanged,
          [this, id](int value) { mBus->dispatch<Events::ModifyActionInputValue>(id, static_cast<float>(value)); });

  connect(mUi->name, &QLineEdit::textEdited,
          [this, id](QString text) { mBus->dispatch<Events::ModifyActionInputName>(id, text.toStdString()); });
}

InputPanel::~InputPanel() = default;

void InputPanel::updateFrom(Ptr<ActionInput const> const& input)
{
  SignalBlocker blocker({ mUi->spinBox, mUi->slider });

  mUi->spinBox->setRange(input->min, input->max);
  mUi->slider->setRange(std::floor(input->min), std::ceil(input->max));
  mUi->spinBox->setValue(input->value);
  mUi->slider->setValue(std::floor(input->value + 0.5f));
}
