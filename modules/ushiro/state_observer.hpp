#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace ushiro
{

template <typename Function, typename Tuple, std::size_t... I> auto call(Function f, Tuple t, std::index_sequence<I...>)
{
  return f(std::get<I>(t)...);
}

template <typename Function, typename Tuple> auto call(Function f, Tuple t)
{
  static constexpr auto size = std::tuple_size<Tuple>::value;
  return call(f, t, std::make_index_sequence<size>{});
}

template <class Model> class state_observation_manager
{
public:
  using ChangeHandler = std::function<void(Model const& current, Model const& previous)>;

  explicit state_observation_manager(Model const& state)
  : m_state(state)
  {
  }

  void message_changed(Model const& from, Model const& to)
  {
    for (auto& each : m_just_added)
    {
      m_list.push_back(std::move(each));
    }
    m_just_added.clear();

    for (auto const& handler : m_list)
    {
      handler(to, from);
    }
  }

  /** Fully configurable version.
   */
  template <class ProjectionType, class WeakEqual, class HandlerType>
  void observe(ProjectionType projection, WeakEqual predicate, HandlerType handler)
  {
    auto change_handler = [projection, predicate, handler](Model const& current_state, Model const& previous_state) {
      auto const& new_value = projection(current_state);
      if (!predicate(new_value, projection(previous_state)))
      {
        handler(new_value);
      }
    };

    // Queue change handler for later
    m_just_added.push_back(change_handler);

    // Do an initial execution of the handler at once
    handler(projection(m_state));
  };

  /** Facade version.
   */
  template <class ProjectionType, class HandlerType> void observe(ProjectionType projection, HandlerType handler)
  {
    observe(projection, std::equal_to<void>(), unwrapping_adaptor<HandlerType>(handler));
  }

private:
  template <class T> class unwrapping_adaptor
  {
  public:
    explicit unwrapping_adaptor(T Handler)
    : m_handler(Handler)
    {
    }

    template <class P> auto operator()(P&& Parameter) const
    {
      return m_handler(std::forward<P>(Parameter));
    }

    template <class... P> auto operator()(std::tuple<P&...> Tuple) const
    {
      return call(m_handler, Tuple);
    }

  private:
    T m_handler;
  };

  Model const& m_state;
  std::vector<ChangeHandler> m_list;
  std::vector<ChangeHandler> m_just_added;
};

template <typename Model> class state_observer
{
public:
  using manager_ptr = std::shared_ptr<state_observation_manager<Model>>;

  explicit state_observer(manager_ptr manager_)
  : m_manager(std::move(manager_))
  {
  }

  state_observer(state_observer&&) noexcept = default;
  state_observer(state_observer const& parent) = default;

  template <class Projection, class WeakEqual, class Handler>
  inline void observe(Projection projection, WeakEqual predicate, Handler handler)
  {
    m_manager->observe(projection, predicate, handler);
  }

  template <class Projection, class Handler> inline void observe(Projection projection, Handler handler)
  {
    m_manager->observe(projection, handler);
  }

private:
  manager_ptr m_manager;
};
} // namespace ushiro
