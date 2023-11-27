#include "state.hpp"

#include <iostream>
#include "node.hpp"
#include "action.hpp"

painlessMesh mesh;
Scheduler userScheduler;

void update_node()
{
    node->m_state->execute_state();
    node->execute_stack();
}

Task task_update_node(TASK_SECOND * 1, TASK_FOREVER, &update_node);

namespace march
{
    // Initialize State =========================================================

    auto initialize::execute_state() -> void
    {
        if (m_node.is_initialized)
            return;

        userScheduler.addTask(task_update_node);
        task_update_node.enable();

        mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);

        mesh.onReceive([this](uint32_t from, String &msg) { m_node.add_action(new recieve_message(m_node, msg)); });

        m_node.m_information = information(mesh.getNodeId());
        m_node.is_initialized = true;

        m_node.print("Node initialized.");

        m_node.add_action(new broadcast_message(m_node, MESSAGE_TYPE::BROADCAST, "Hello there!"));
        m_node.execute_stack();

        m_node.change_state(new idle(m_node));
    }

    // Idle State =========================================================

    auto idle::execute_state() -> void
    {
        m_node.print("Idle state.");
        // m_node.add_action(new broadcast_message(m_node, MESSAGE_TYPE::BROADCAST, "Hello there!"));
    }

    // Charging State =========================================================

    auto charging::execute_state() -> void
    {
    }

    auto moving::execute_state() -> void
    {
    }
}
