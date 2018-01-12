#pragma once
#include <optional.hpp>

namespace view
{

template <typename Function, typename Tuple, size_t... I> auto call(Function f, Tuple t, std::index_sequence<I...>)
{
    return f(std::get<I>(t)...);
}

template <typename Function, typename Tuple> auto call(Function f, Tuple t)
{
    static constexpr auto size = std::tuple_size<Tuple>::value;
    return call(f, t, std::make_index_sequence<size>{});
}

template <class Model> class binder
{
public:
    enum class update
    {
        initial,
        changed
    };

    using error_handler = std::function<void(std::exception const&, update)>;

    explicit binder(error_handler handler_ = {})
    : m_error_handler(std::move(handler_))
    {
    }

    void operator()(Model const& model)
    {
        if (!m_previous)
        {
            for (auto const& handler : mForcedHandlers)
            {
                try
                {
                    handler(model);
                }
                catch (std::exception const& e)
                {
                    on_error(e, update::initial);
                }
            }
        }
        else
        {
            for (auto const& handler : mDiffingHandlerList)
            {
                try
                {
                    handler(model, *m_previous);
                }
                catch (std::exception const& e)
                {
                    on_error(e, update::changed);
                }
            }
        }
        m_previous = model;
    }

    /** Fully configurable version.
     */
    template <class ProjectionType, class WeakEqual, class HandlerType>
    inline void connect(ProjectionType Projection, WeakEqual Predicate, HandlerType Handler)
    {
        mDiffingHandlerList.push_back([Projection, Predicate, Handler](Model const& Current, Model const& Previous) {
            auto const& NewValue = Projection(Current);
            if (!Predicate(NewValue, Projection(Previous)))
            {
                Handler(NewValue);
            }
        });

        mForcedHandlers.push_back([Projection, Handler](Model const& Current) { Handler(Projection(Current)); });
    };

    /** Facade version.
     */
    template <class ProjectionType, class HandlerType>
    inline void connect(ProjectionType Projection, HandlerType Handler)
    {
        using ProjectedType = decltype(Projection(*mPrevious));
        connect(Projection, std::equal_to<ProjectedType>(), UnwrappingAdaptor<HandlerType>(Handler));
    }

private:
    void on_error(std::exception const& e, update update_)
    {
        if (m_error_handler)
        {
            m_error_handler(e, update_);
        }
    }

    template <class T> class UnwrappingAdaptor
    {
    public:
        UnwrappingAdaptor(T Handler)
        : mHandler(Handler)
        {
        }

        template <class P> auto operator()(P Parameter) const
        {
            return mHandler(std::forward<P>(Parameter));
        }

        template <class... P> auto operator()(std::tuple<P...> Tuple) const
        {
            return call(mHandler, Tuple);
        }

    private:
        T mHandler;
    };

    error_handler m_error_handler;
    std::vector<std::function<void(Model const& current, Model const& previous)>> mDiffingHandlerList;
    std::vector<std::function<void(Model const& Current)>> mForcedHandlers;
    nonstd::optional<Model> m_previous;
};

template <typename Action, typename Model> class component
{
public:
    using dispatch_t = std::function<void(Action)>;

    component(dispatch_t dispatch_, std::shared_ptr<binder<Model>> binder_)
    : m_dispatch(std::move(dispatch_))
    , m_binder(std::move(binder_))
    {
    }

    component(component const& parent) = default;

    template <class Projection, class WeakEqual, class Handler>
    inline void connect_view(Projection projection, WeakEqual predicate, Handler handler)
    {
        m_binder->connect(projection, predicate, handler);
    }

    template <class Projection, class Handler> inline void connect_view(Projection projection, Handler handler)
    {
        m_binder->connect(projection, handler);
    }

    inline void dispatch(Action action)
    {
        m_dispatch(action);
    }

private:
    dispatch_t m_dispatch;
    std::shared_ptr<binder<Model>> m_binder;
};
}