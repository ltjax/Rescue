#include "SignalBlocker.hpp"

SignalBlocker::SignalBlocker(std::initializer_list<QObject*> widgets)
{
  mBlockers.reserve(widgets.size());
  for (auto each : widgets)
  {
    mBlockers.emplace_back(each);
  }
}

SignalBlocker::~SignalBlocker() = default;

