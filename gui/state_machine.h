#pragma once

#include <assert.h>
#include <functional>
#include <memory>
#include <unordered_map>

namespace sf { class Event; }


struct state_abstract
{
    virtual ~state_abstract() = default;
    virtual void on_enter() = 0;
    virtual void on_exit() = 0;
    virtual void on_update() = 0;
    virtual void on_render() = 0;
    virtual void on_gui() = 0;
    virtual void on_event(const sf::Event& event) = 0;
};

template<typename T>
struct state_wrapper : public state_abstract
{
    explicit state_wrapper(T type) : m_type(type) {}
    void on_enter() override { m_type.on_enter(); }
    void on_exit() override { m_type.on_exit(); }
    void on_update() override { m_type.on_update(); }
    void on_render() override { m_type.on_render(); }
    void on_gui() override { m_type.on_gui(); }
    void on_event(const sf::Event& event) override { m_type.on_event(event); }

    template <typename... Args>
    void set_params(Args&& ...args)
    {
        m_type.set_params(args...);
    }

private:
    T m_type;
};

struct state_handler
{
    template <typename T>
    state_handler(T type)
    {
        m_type = std::make_unique<state_wrapper<T>>(type);
    }

    template <typename T, typename... Args>
    void set_params(Args&& ...args)
    {
        static_cast<state_wrapper<T>&>(*m_type).set_params(args...);
    }

    void on_enter() { m_type->on_enter(); }
    void on_exit() { m_type->on_exit(); }
    void on_update() { m_type->on_update(); }
    void on_render() { m_type->on_render(); }
    void on_gui() { m_type->on_gui(); }
    void on_event(const sf::Event& event) { m_type->on_event(event); }

private:
    std::unique_ptr<state_abstract> m_type;
};


template<typename State>
class state_machine
{
public:
    template <typename T, typename... Args>
    void register_handler(State state, Args&& ...args)
    {
        m_handler.emplace(state, state_handler( T(args...) ));
    }

    void register_transition(State from, State to, const std::function<void()>& action = [](){})
    {
        m_transition[from][to] = action;
    }

    void transition(State new_state)
    {
        assert(m_transition[m_current].find(new_state) != m_transition[m_current].end());

        m_transition[m_current][new_state]();
        handler(m_current).on_exit();
        m_current = new_state;
        handler(new_state).on_enter();
    }

    state_handler& handler(State state)
    {
        auto handler_iter = m_handler.find(state);
        assert(handler_iter != m_handler.end());
        return handler_iter->second;
    }

    state_handler& current_handler()
    {
        return handler(m_current);
    }

    void start(State current)
    {
        m_current = current;
        handler(m_current).on_enter();
    }

private:
    std::unordered_map<State, std::unordered_map<State, std::function<void()>>> m_transition;
    std::unordered_map<State, state_handler> m_handler;
    State m_current;
};

