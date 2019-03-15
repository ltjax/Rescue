#pragma once

#include <utility>
#include <exception>
#include <functional>
#include <vector>
#include <memory>

namespace ushiro
{

template<typename Function, typename Tuple, std::size_t... I>
auto call(Function f, Tuple t, std::index_sequence<I...>)
{
  return f(std::get<I>(t)...);
}

template<typename Function, typename Tuple>
auto call(Function f, Tuple t)
{
  static constexpr auto size = std::tuple_size<Tuple>::value;
  return call(f, t, std::make_index_sequence < size > {});
}

template<class Model>
class state_observation_manager
{
public:
  enum class update
  {
    initial,
    changed
  };

  using ChangeHandler = std::function<void(Model const& current, Model const& previous)>;

  using error_handler = std::function<void(std::exception const&, update)>;
  using get_state = std::function<Model const&()>;

  explicit state_observation_manager(get_state get_state_, error_handler handler_ = {})
    : m_get_state(get_state_), m_error_handler(std::move(handler_))
  {
  }

  void message_changed(Model const& from, Model const& to)
  {
    for (auto& each : m_just_added)
      m_list.push_back(std::move(each));

    for (auto const& handler : m_list)
    {
      invoke(handler, to, from);
    }
  }

  void invoke(ChangeHandler const& handler, Model const& to, Model const& from)
  {
    try
    {
      handler(to, from);
    }
    catch (std::exception const& e)
    {
      on_error(e, update::changed);
    }
  }

  /** Fully configurable version.
   */
  template<class ProjectionType, class WeakEqual, class HandlerType>
  inline void observe(ProjectionType Projection, WeakEqual Predicate, HandlerType Handler)
  {
    m_just_added.push_back([Projection, Predicate, Handler](Model const& Current, Model const& Previous)
                                  {
                                    auto const& NewValue = Projection(Current);
                                    if (!Predicate(NewValue, Projection(Previous)))
                                    {
                                      Handler(NewValue);
                                    }
                                  });

    // Do an initial execution of the handler at once
    if (m_get_state)
    {
      try
      {
        Handler(Projection(m_get_state()));
      }
      catch (std::exception const& e)
      {
        on_error(e, update::initial);
      }
    }
  };

  /** Facade version.
   */
  template<class ProjectionType, class HandlerType>
  inline void observe(ProjectionType Projection, HandlerType Handler)
  {
    observe(Projection, std::equal_to<void>(), unwrapping_adaptor<HandlerType>(Handler));
  }

private:
  void on_error(std::exception const& e, update update_)
  {
    if (m_error_handler)
    {
      m_error_handler(e, update_);
    }
  }

  template<class T>
  class unwrapping_adaptor
  {
  public:
    explicit unwrapping_adaptor(T Handler)
      : m_handler(Handler)
    {
    }

    template<class P>
    auto operator()(P&& Parameter) const
    {
      return m_handler(std::forward<P>(Parameter));
    }

    template<class... P>
    auto operator()(std::tuple<P&...> Tuple) const
    {
      return call(m_handler, Tuple);
    }

  private:
    T m_handler;
  };

  get_state m_get_state;
  error_handler m_error_handler;
  std::vector<ChangeHandler> m_list;
  std::vector<ChangeHandler> m_just_added;
};

template<typename Model>
class state_observer
{
public:
  using manager_ptr = std::shared_ptr<state_observation_manager<Model>>;

  explicit state_observer(manager_ptr manager_)
    : m_manager(std::move(manager_))
  {
  }

  state_observer(state_observer&&) noexcept = default;
  state_observer(state_observer const& parent) = default;

  template<class Projection, class WeakEqual, class Handler>
  inline void observe(Projection projection, WeakEqual predicate, Handler handler)
  {
    m_manager->observe(projection, predicate, handler);
  }

  template<class Projection, class Handler>
  inline void observe(Projection projection, Handler handler)
  {
    m_manager->observe(projection, handler);
  }

private:
  manager_ptr m_manager;
};
}
