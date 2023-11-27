#if !defined(MARCH_STATE_H_)
#define MARCH_STATE_H_

#include <string>

#include "action.hpp"
#include "painlessMesh.h"

extern Task task_update_node;
extern march::node *node;

namespace march
{
    class node;

    enum STATE_TYPE
    {
        INIT,
        IDLE,
        CHARGING,
        MOVE_TO
    };

    // =========================================================
    // STATE SUPERCLASS
    // =========================================================

    /**
     * IMPLEMENTS STATE PATTERN
     */
    class state
    {

    public:
        state(STATE_TYPE type, march::node &node_) : m_type(type), m_node(node_){};
        virtual ~state() = default;

    public:
        STATE_TYPE get_type() const { return m_type; };
        virtual std::string get_name() const = 0;
        virtual void execute_state() = 0;

    protected:
        STATE_TYPE m_type;
        node &m_node;
    };

    // =========================================================
    // STATE SUBCLASSES
    // =========================================================

    class initialize : public state
    {
    public:
        initialize(node &node) : state(STATE_TYPE::INIT, node){};
        ~initialize(){};

    public:
        void execute_state();
        std::string get_name() const { return "INITIALIZE"; };
    };

    // =========================================================

    class idle : public state
    {
    public:
        idle(node &node) : state(STATE_TYPE::IDLE, node){};
        ~idle(){};

    public:
        void execute_state();
        std::string get_name() const { return "IDLE"; };
    };

    // =========================================================

    class charging : public state
    {
    public:
        charging(node &node) : state(STATE_TYPE::CHARGING, node){};
        ~charging(){};

    public:
        void execute_state();
        std::string get_name() const { return "CHARGING"; };
    };

    // =========================================================

    class moving : public state
    {
    public:
        moving(node &node, int new_position) : state(STATE_TYPE::MOVE_TO, node), m_new_position(new_position){};
        ~moving(){};

    public:
        void execute_state();
        std::string get_name() const { return "MOVING TO " + std::to_string(m_new_position); };

    private:
        int m_new_position;
    };

}

#endif // MARCH_STATE_H_
