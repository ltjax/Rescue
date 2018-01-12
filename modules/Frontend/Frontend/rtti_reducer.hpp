#pragma once
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

template <class Model> class rtti_reducer
{
public:
    struct wrapped_base
    {
    public:
        virtual ~wrapped_base(){};

    protected:
        wrapped_base() = default;
    };

    template <typename Contained> struct wrapped : public wrapped_base
    {
        wrapped(Contained c)
        : contained(std::move(c))
        {
        }
        Contained contained;
    };

    class action
    {
    public:
        template <class T>
        action(T original)
        : m_pointee(std::make_shared<wrapped<T>>(original))
        {
        }

        action(action const&) = default;
        action(action&&) = default;

        wrapped_base& get() const
        {
            return *m_pointee;
        }

    private:
        std::shared_ptr<wrapped_base> m_pointee;
    };

    Model operator()(Model model, action which) const
    {
        auto key = std::type_index(typeid(which.get()));
        auto found = m_handler.find(key);
        if (found == m_handler.end())
            return model;
        return found->second(model, which.get());
    }

    template <class Action, class Handler> void subscribe(Handler handler)
    {
        using result_type = typename std::result_of<Handler(Model, Action)>::type;
        static_assert(std::is_same<result_type, Model>::value, "Reducers needs to return the model");

        m_handler[std::type_index(typeid(wrapped<Action>))] = [handler](Model model, wrapped_base const& inner) {
            Action const& action = static_cast<wrapped<Action> const&>(inner).contained;
            return handler(model, action);
        };
    }

private:
    using handler_type = std::function<Model(Model, wrapped_base const&)>;
    std::unordered_map<std::type_index, handler_type> m_handler;
};
