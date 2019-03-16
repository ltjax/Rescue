#include "InputPanelListWidget.hpp"

using namespace Rescue;

InputPanelListWidget::InputPanelListWidget(Rescue::Ptr<ushiro::event_bus> bus,
                                           ushiro::state_observer<Rescue::State> observer,
                                           QWidget* parent)
: QWidget(parent)
, mBus(std::move(bus))
, mObserver(std::move(observer))
{
  mObserver.observe([](State const& state) { return state.inputs; }, [this](auto const& inputs) { updateFrom(inputs); });
  mLayout = new QVBoxLayout();
  mLayout->addStretch();
  mLayout->setMargin(0);
  this->setLayout(mLayout);
}

void InputPanelListWidget::updateFrom(Inputs const& inputs)
{
  mWidgets.update(inputs, [this](Ptr<ActionInput const> const& input, int index) {
    auto widget = new InputPanel(mBus, mObserver, input->id, this);
    mLayout->insertWidget(index, widget);
    return widget;
  });
}
