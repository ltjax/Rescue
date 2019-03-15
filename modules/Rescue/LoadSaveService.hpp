#pragma once

#include "event_bus.hpp"
#include "store.hpp"
#include "Vocabulary.hpp"
#include "State.hpp"

namespace Rescue
{

class LoadSaveService
{
public:
  explicit LoadSaveService(Ptr<ushiro::event_bus> bus, ushiro::store<State> const& store);

private:
  Ptr<ushiro::event_bus> mBus;
  ushiro::store<State> const& mStore;
  ushiro::event_bus::subscription_list mSubscriptions;
};

}



