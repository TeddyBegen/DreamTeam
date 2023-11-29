#if !defined(STATE_HPP_)
#define STATE_HPP_

#include <string>
#include <memory>
#include <stack>
#include <iostream>
#include <unordered_map>
#include <ArduinoJson.h>

#include "action.hpp"

extern std::shared_ptr<dteam::node> node_ptr;

using node_id = int;
using node_score = int;

namespace dteam
{
    class node;

    enum class state_type
    {
        idle,
        initializing
    };

    /**
     * @brief This class represents the state of an agent.
     */
    class state
    {
    public:
        state(dteam::state_type type, std::shared_ptr<dteam::node> node) : type(type), m_node(node){};
        virtual ~state() = default;

    public:
        virtual auto enter() -> void = 0;
        virtual auto execute() -> void = 0;
        virtual auto exit() -> void = 0;

        virtual auto to_string() const -> std::string = 0;
        virtual auto recieve_callback(JsonObject json) -> void;

    protected:
        auto run_stack() -> void
        {
            if (m_actions.empty())
                return;

            while (!m_actions.empty())
            {
                m_actions.top()->execute();
                m_actions.pop();
            }
        }

    protected:
        dteam::state_type type;
        std::shared_ptr<dteam::node> m_node;
        std::stack<std::unique_ptr<dteam::action>> m_actions;
    };

    /**
     * @brief This class represents the initializing state.
     *
     * An agent begins its lifecycle in this state and transitions to the idle state
     * once it has finished setting up its information & communication modules.
     *
     */
    class initializing_state : public dteam::state
    {
    public:
        initializing_state(std::shared_ptr<dteam::node> node) : dteam::state(dteam::state_type::initializing, node){};
        ~initializing_state(){};

    public:
        auto enter() -> void override;
        auto execute() -> void override;
        auto exit() -> void override;

        auto to_string() const -> std::string override { return "initializing"; }
    };

    /**
     * @brief This class represents the idle state.
     *
     * An agent is in this state when it is not performing any actions.
     *
     */
    class idle_state : public dteam::state
    {
    public:
        idle_state(std::shared_ptr<dteam::node> node) : dteam::state(dteam::state_type::idle, node){};
        ~idle_state(){};

    public:
        auto enter() -> void override;
        auto execute() -> void override;
        auto exit() -> void override;

        auto to_string() const -> std::string override { return "idle"; }
    };

    class main_mission_state : public dteam::state
    {
    public:
        main_mission_state(std::shared_ptr<dteam::node> node) : dteam::state(dteam::state_type::idle, node){};
        ~main_mission_state(){};

    public:
        auto enter() -> void override;
        auto execute() -> void override;
        auto exit() -> void override;

        auto to_string() const -> std::string override { return "main_mission"; }

    private:
        bool m_levels_ok = false;
        bool m_transport_done = false;
    };

    class transport_state : public dteam::state
    {
    public:
        transport_state(std::shared_ptr<dteam::node> node) : dteam::state(dteam::state_type::idle, node){};
        ~transport_state(){};

    public:
        auto enter() -> void override;
        auto execute() -> void override;
        auto exit() -> void override;

        auto to_string() const -> std::string override { return "transport"; }
    };

    class charging_mission_state : public dteam::state
    {
    public:
        charging_mission_state(std::shared_ptr<dteam::node> node) : dteam::state(dteam::state_type::idle, node){};
        ~charging_mission_state(){};

    public:
        auto enter() -> void override;
        auto execute() -> void override;
        auto exit() -> void override;

        auto to_string() const -> std::string override { return "charging_mission"; }

    private:
        int m_charging_station_position;
    };

    class arrival_state : public dteam::state
    {
    public:
        arrival_state(std::shared_ptr<dteam::node> node) : dteam::state(dteam::state_type::idle, node){};
        ~arrival_state(){};

    public:
        auto enter() -> void override;
        auto execute() -> void override;
        auto exit() -> void override;

        auto to_string() const -> std::string override { return "arrival"; };
        auto recieve_callback(JsonObject json) -> void;

    private:
        std::unordered_map<int, int> m_node_scores;
        bool m_ready_to_send_queue_numbers = false;
    };

    class queue_state : public dteam::state
    {
    public:
        queue_state(std::shared_ptr<dteam::node> node) : dteam::state(dteam::state_type::idle, node){};
        ~queue_state(){};

    public:
        auto enter() -> void override;
        auto execute() -> void override;
        auto exit() -> void override;

        auto to_string() const -> std::string override { return "queue"; };
        auto recieve_callback(JsonObject json) -> void;
    };

    class charging_state : public dteam::state
    {
    public:
        charging_state(std::shared_ptr<dteam::node> node) : dteam::state(dteam::state_type::idle, node){};
        ~charging_state(){};

    public:
        auto enter() -> void override;
        auto execute() -> void override;
        auto exit() -> void override;

        auto to_string() const -> std::string override { return "charging"; };
        // auto recieve_callback(JsonObject json) -> void;
    };

}

#endif // STATE_HPP_
