#pragma once
#include <functional>
#include <typeindex>
#include <unordered_map>

template <class Model> class rtti_reducer
{
public:
    struct wrapped_base
    {
        virtual ~wrapped_base() = default;
    };

    template <typename Contained> struct wrapped : public wrapped_base
    {
        Contained contained;
    };

    class action
    {
    public:
        template <class T>
        action(T const& original)
        : m_pointee(&original)
        {
        }

        action(action const&) = default;

        wrapped_base& get() const
        {
            return *m_pointee;
        }

    private:
        wrapped_base* m_pointee;
    };

    Model operator()(Model model, action which) const
    {
        auto found = m_handler.find(std::type_index(typeid(which.get())));
        if (found != m_handler.end())
            return model;
        return found->second(model, which.get());
    }

    template <class Action, class Handler> rtti_reducer subscribe(Handler handler)
    {
        m_handler[std::type_index(typeid(Action))] = [handler](Model model, action which) {
            return handler(model, static_cast<wrapped<Action> const&>(which.get()).contained);
        };
        return this;
    }

private:
    using handler_type = std::function<Model(Model, wrapped_base const&)>;
    std::unordered_map<std::type_index, handler_type> m_handler;
};
