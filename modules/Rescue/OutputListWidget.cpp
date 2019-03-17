#include "OutputListWidget.hpp"

using namespace Rescue;

OutputListWidget::OutputListWidget(ushiro::state_observer<Rescue::State> observer, QWidget* parent)
: QWidget(parent)
, mObserver(std::move(observer))
{
  mLayout = new QFormLayout(this);
  setLayout(mLayout);

  mObserver.observe([](State const& state) { return std::tie(state.outputs, state.group); },
                    [this](Outputs const& outputs, Group const& group) { return updateFrom(outputs, group); });
}

OutputListWidget::~OutputListWidget() = default;

void OutputListWidget::updateFrom(Outputs const& outputs, Group const& group)
{
  // Clear all
  while (mLayout->rowCount())
  {
    mLayout->removeRow(mLayout->rowCount() - 1);
  }

  for (auto const& output : outputs)
  {
    auto name = locate(group, output->id)->name;
    mLayout->addRow(new QLabel(name.c_str(), this), new QLabel(QString("%1").arg(output->value), this));
  }
}
