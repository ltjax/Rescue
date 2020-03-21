#pragma once

#include <ushiro/event_bus.hpp>
#include <ushiro/store.hpp>
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
  ushiro::subscription_list mSubscriptions;
};

}



