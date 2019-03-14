#pragma once

namespace ushiro
{
template <typename Store>
class EventBinder
{
public:
  EventBinder(Ptr<event_bus> const& bus, Store& store) : bus(bus), store(store)
  {
  }

  template <typename T>
  EventBinder bind()
  {
    bus->subscribe<T>(event_applier(store));
    return *this;
  }

private:
  Ptr<event_bus> const& bus;
  Store& store;
};

template <typename Store>
EventBinder<Store> events(Ptr<event_bus> const& bus, Store& store)
{
  return EventBinder<Store>(bus, store);
}

}