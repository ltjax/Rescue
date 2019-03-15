#pragma once

#include <QtWidgets/QWidget>

class SignalBlocker
{
public:
  SignalBlocker(std::initializer_list<QObject*> widgets);
  ~SignalBlocker();

private:
  std::vector<QSignalBlocker> mBlockers;

};



