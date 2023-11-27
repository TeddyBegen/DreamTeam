#if !defined(MARCH_NODE_H_)
#define MARCH_NODE_H_

#define MESH_PREFIX "dreamteamnetwork"
#define MESH_PASSWORD "password"
#define MESH_PORT 5555

#include <string>
#include <iostream>
#include <vector>

#include "state.hpp"
#include "information.hpp"
#include "action.hpp"

#include "painlessMesh.h"

extern painlessMesh mesh;

namespace march
{
    class node
    {
    public:
        node() : m_state(new initialize(*this)), m_action_stack({}), m_messages({}), m_mesh(mesh){};
        ~node()
        {
            delete m_state;
            for (auto message : m_messages)
                delete message;
            for (auto action : m_action_stack)
                delete action;
        };

    public:
        void print(std::string message) { std::cout << "ID:\t\t" << m_information.get_ID() << "\n"
                                                    << "Current State:\t" << m_state->get_name() << "\n"
                                                    << "Printing:\t" << message << "\n\n"
                                                    << std::endl; }

    public:
        void add_action(march::action *action) { m_action_stack.push_back(action); };
        void execute_stack()
        {
            while (!m_action_stack.empty())
            {
                m_action_stack.back()->execute();
                delete m_action_stack.back();
                m_action_stack.pop_back();
            }
        };

        void change_state(march::state *new_state)
        {
            delete m_state;
            m_state = new_state;
        };

    public:
        march::information m_information;
        std::vector<march::message *> m_messages;
        march::state *m_state;

    public:
        painlessMesh &m_mesh;
        std::vector<march::action *> m_action_stack;

    public:
        bool is_initialized = false;

    private:
        int lifetime = 0;
    };
}

#endif // MARCH_NODE_H_
