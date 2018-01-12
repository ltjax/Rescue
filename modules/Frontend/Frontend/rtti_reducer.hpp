#pragma once
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

struct rtti_action_wrapper_base
{
public:
    virtual ~rtti_action_wrapper_base(){};

protected:
    rtti_action_wrapper_base() = default;
};

template <typename Contained> struct rtti_action_wrapper : public rtti_action_wrapper_base
{
    rtti_action_wrapper(Contained c)
    : contained(std::move(c))
    {
    }
    Contained contained;
};

template <class Model> class rtti_reducer
{
public:
    class action
    {
    public:
        template <class T>
        action(T original)
        : m_pointee(std::make_shared<rtti_action_wrapper<T>>(original))
        {
        }

        action(action const&) = default;
        action(action&&) = default;

        rtti_action_wrapper_base& get() const
        {
            return *m_pointee;
        }

    private:
        std::shared_ptr<rtti_action_wrapper_base> m_pointee;
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

        m_handler[std::type_index(typeid(rtti_action_wrapper<Action>))] =
            [handler](Model model, rtti_action_wrapper_base const& inner) {
                Action const& action = static_cast<rtti_action_wrapper<Action> const&>(inner).contained;
                return handler(model, action);
            };
    }

private:
    using handler_type = std::function<Model(Model, rtti_action_wrapper_base const&)>;
    std::unordered_map<std::type_index, handler_type> m_handler;
};
