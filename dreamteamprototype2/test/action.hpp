#if !defined(MARCH_ACTION_H_)
#define MARCH_ACTION_H_

#include <string>

#include "message.hpp"
#include "WString.h"

namespace march
{

    class node;

    enum ACTION_TYPE
    {
        UPDATE_BELIEFS,
        CHARGE_BATTERY,
        BROADCAST_MESSAGE,
        RECIEVE_MESSAGE,
        MOVING
    };

    // =========================================================
    // ACTION SUPERCLASS
    // =========================================================

    /**
     * IMPLEMENTS STRATEGY PATTERN
     */
    class action
    {
    public:
        action(march::node &node, ACTION_TYPE type) : m_node(node){};
        virtual ~action() = default;

    public:
        virtual void execute() = 0;

    protected:
        march::node &m_node;
        ACTION_TYPE m_type;
    };

    // =========================================================
    // ACTION SUBCLASSES
    // =========================================================

    class recieve_message : public action
    {
    public:
        recieve_message(march::node &node, String message) : action(node, ACTION_TYPE::RECIEVE_MESSAGE), m_message(message){};
        ~recieve_message(){};

    public:
        void execute();

    private:
        String m_message;
    };

    

    class update_beliefs : public action
    {
    public:
        update_beliefs(march::node &node) : action(node, ACTION_TYPE::UPDATE_BELIEFS){};
        ~update_beliefs(){};

    public:
        void execute();
    };

    class charge_battery : public action
    {
    public:
        charge_battery(march::node &node) : action(node, ACTION_TYPE::CHARGE_BATTERY){};
        ~charge_battery(){};

    public:
        void execute();
    };

    class broadcast_message : public action
    {
    public:
        broadcast_message(march::node &node, march::MESSAGE_TYPE type, std::string message)
            : action(node, ACTION_TYPE::BROADCAST_MESSAGE), m_message(message), m_message_type(type){};
        ~broadcast_message(){};

    public:
        void execute();

    private:
        std::string m_message;
        march::MESSAGE_TYPE m_message_type;
    };

    class move_to : public action
    {
    public:
        move_to(march::node &node, int new_position) : action(node, ACTION_TYPE::MOVING), m_new_position(new_position){};
        ~move_to(){};

    public:
        void execute();

    private:
        int m_new_position;
    };

} // namespace march

#endif // MARCH_ACTION_H_