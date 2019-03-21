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

template <class Model> class observation_manager
{
public:
  using ChangeHandler = std::function<void(Model const& current, Model const& previous)>;
  using HandlerList = std::list<ChangeHandler>;

  struct token
  {
    typename HandlerList::iterator detail;
  };

  explicit observation_manager(Model const& state)
  : m_state(state)
  {
  }

  void message_changed(Model const& from, Model const& to)
  {
    m_list.splice(m_list.end(), m_just_added);

    for (auto const& handler : m_list)
    {
      handler(to, from);
    }
  }

  /** Fully configurable version.
   */
  template <class ProjectionType, class WeakEqual, class HandlerType>
  token observe(ProjectionType projection, WeakEqual predicate, HandlerType handler)
  {
    auto change_handler = [projection, predicate, handler](Model const& current_state, Model const& previous_state) {
      auto const& new_value = projection(current_state);
      if (!predicate(new_value, projection(previous_state)))
      {
        handler(new_value);
      }
    };

    // Queue change handler for later
    auto where = m_just_added.insert(m_just_added.end(), change_handler);

    // Do an initial execution of the handler at once
    handler(projection(m_state));

    return token{where};
  };

  /** Facade version.
   */
  template <class ProjectionType, class HandlerType> token observe(ProjectionType projection, HandlerType handler)
  {
    return observe(projection, std::equal_to<void>(), unwrapping_adaptor<HandlerType>(handler));
  }

  void forget(token what)
  {
    m_list.erase(what.detail);
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
  std::list<ChangeHandler> m_list;
  std::list<ChangeHandler> m_just_added;
};

template <class Model>
using link = std::shared_ptr<observation_manager<Model>>;

template <typename Model> class state_observer
{
public:

  explicit state_observer(link<Model> link_)
  : m_link(std::move(link_))
  {
  }

  ~state_observer()
  {
    if (!m_link)
      return;

    for (auto const& each : m_connections)
      m_link->forget(each);
  }

  // Movable...
  state_observer(state_observer&&) noexcept = default;
  state_observer& operator=(state_observer&&) noexcept = default;

  // but not copyable!
  state_observer(state_observer const&) = delete;
  state_observer& operator=(state_observer const&) = delete;

  template <class Projection, class WeakEqual, class Handler>
  inline void observe(Projection projection, WeakEqual predicate, Handler handler)
  {
    m_connections.push_back(m_link->observe(projection, predicate, handler));
  }

  template <class Projection, class Handler> inline void observe(Projection projection, Handler handler)
  {
    m_connections.push_back(m_link->observe(projection, handler));
  }

  link<Model> manager() const
  {
    return m_link;
  }

private:
  using token = typename observation_manager<Model>::token;
  link<Model> m_link;
  std::vector<token> m_connections;
};
} // namespace ushiro
