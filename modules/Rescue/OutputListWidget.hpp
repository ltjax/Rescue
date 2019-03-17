#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QFormLayout>
#include <diffable_list.hpp>
#include <QtWidgets/QLabel>
#include "state_observer.hpp"
#include "State.hpp"

namespace Rescue
{

class OutputListWidget : public QWidget
{
public:
  OutputListWidget(ushiro::state_observer<State> observer, QWidget* parent);
  ~OutputListWidget();

private:
  void updateFrom(Outputs const& outputs, Group const& group);
  ushiro::state_observer<Rescue::State> mObserver;
  QFormLayout* mLayout;
};

}




