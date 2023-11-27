#include "action.hpp"

#include "node.hpp"
#include "message.hpp"

extern std::vector<march::message *> message_buffer;

namespace march
{
    // =========================================================
    // ACTION SUBCLASSES
    // =========================================================

    void recieve_message::execute()
    {
        m_node.print("Recieved message, will parse and add to buffer");

        DynamicJsonDocument jsonBuffer(1024 + m_message.length());
        DeserializationError error = deserializeJson(jsonBuffer, m_message);

        if (error)
        {
            m_node.print("Failed to parse JSON message, deserialization error: " + std::string(error.c_str()));
            return;
        }

        m_node.print("Recieved: " + jsonBuffer["body"]);
    }

    void update_beliefs::execute()
    {
        m_node.print("Updating beliefs");

        // // Eg. battery level, location, etc.

        for (auto &message : message_buffer)
        {
            switch (message->m_type)
            {
            case MESSAGE_TYPE::BROADCAST:
            {
                auto broadcasted_message = static_cast<march::global_message *>(message);

                if (broadcasted_message->m_body == "DONE CHARGING")
                {
                    m_node.change_state(new march::moving(m_node, broadcasted_message->m_sender_information.get_position()));
                    return;
                }

                break;
            }

            default:
                break;
            }
        }

        return;
    }

    void charge_battery::execute()
    {
        // TEMPORARY

        int new_battery_level = m_node.m_information.get_battery_level() + 10;
        m_node.m_information.update_battery_level(new_battery_level);

        m_node.print("Battery level: " + std::to_string(m_node.m_information.get_battery_level()));
    }

    void broadcast_message::execute()
    {
        m_node.print("Broadcasting message");
        mesh.sendBroadcast(m_message.c_str());
    }

    void move_to::execute()
    {
        m_node.print("Moving closer to " + std::to_string(m_new_position) + " (current position: " + std::to_string(m_node.m_information.get_position()) + ")");

        int direction = 1;

        if (m_new_position < m_node.m_information.get_position())
            direction = -1;

        m_node.m_information.update_position(m_node.m_information.get_position() + direction);
    }

}
