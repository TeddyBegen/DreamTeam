#if !defined(ACTION_HPP_)
#define ACTION_HPP_

#include <memory>

namespace dteam
{
    class node;
    class state;

    enum class action_type
    {
        state_action,
        broadcast_message_action
    };

    /**
     * @brief This class represents an action that an agent can perform.
     */
    class action
    {
    public:
        action(std::shared_ptr<dteam::node> node, action_type type) : m_node(node){};
        virtual ~action() = default;

    public:
        virtual void execute() = 0;

    protected:
        std::shared_ptr<dteam::node> m_node;
        action_type m_type;
    };

    /**
     * @brief This class represents an action where an agent jumps into a state, executes it and then jumps out.
     */
    class state_action : public dteam::action
    {
    public:
        state_action(std::shared_ptr<dteam::node> node, std::unique_ptr<dteam::state> state)
            : dteam::action(node, action_type::state_action), m_state(std::move(state)){};
        ~state_action() = default;

    public:
        void execute() override;

    private:
        std::unique_ptr<dteam::state> m_state;
    };

    /**
     * @brief This class represents a broadcast message action, where an agent broadcasts a message to all other agents.
     */
    class broadcast_message_action : public dteam::action
    {
    public:
        broadcast_message_action(std::shared_ptr<dteam::node> node, std::string message)
            : dteam::action(node, action_type::broadcast_message_action), m_message(message){};
        ~broadcast_message_action() = default;

    public:
        void execute() override;

    private:
        std::string m_message;
    };

}

#endif // ACTION_HPP_
