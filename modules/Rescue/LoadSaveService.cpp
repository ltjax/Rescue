
#include "LoadSaveService.hpp"
#include "Events.hpp"
#include "LoadSave.hpp"

using namespace Rescue;

LoadSaveService::LoadSaveService(Ptr<ushiro::event_bus> bus, ushiro::store<State> const& store)
: mBus(std::move(bus))
, mStore(store)
{
  mSubscriptions += mBus->subscribe([this](Events::LoadFrom const& event) {
    auto loaded = LoadSave::loadFrom(event.filename);
    mBus->dispatch<Events::Loaded>(loaded);
  });

  mSubscriptions += mBus->subscribe([this](Events::SaveTo const& event) {
    // Get current state from store
    LoadSave::saveTo(event.filename, mStore.state.group);
  });
}
