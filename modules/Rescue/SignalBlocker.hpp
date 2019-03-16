#pragma once

#include <QtWidgets/QWidget>

namespace Rescue
{
class SignalBlocker
{
public:
  SignalBlocker(std::initializer_list<QObject*> widgets);
  ~SignalBlocker();

private:
  std::vector<QSignalBlocker> mBlockers;

};

}



