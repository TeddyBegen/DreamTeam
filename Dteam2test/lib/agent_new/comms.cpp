#include <iostream>

#include "comms.hpp"
#include "node.hpp"

#ifdef ARDUINO

#include "painlessMesh.h"

painlessMesh mesh;
Scheduler userScheduler;

#else
int id_counter = 0;
#endif

Task task_update_node(TASK_SECOND * 1, TASK_FOREVER, []()
                      { node_ptr.get()->get_state().execute(); });

namespace dteam
{

#ifdef ARDUINO

    auto real_comms::initialize(std::shared_ptr<dteam::node> node_ptr) -> void
    {
        set_node(node_ptr);

        mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
        userScheduler.addTask(task_update_node);
        task_update_node.enable();

        mesh.onReceive([this](uint32_t from, String &msg)
                       { 
                        DynamicJsonDocument jsonBuffer(1024 + msg.length());
                        DeserializationError error = deserializeJson(jsonBuffer, msg);

                        if (error)
                        {
                            Serial.printf("DeserializationError\n");
                            return;
                        }

                        auto json = jsonBuffer.as<JsonObject>();

                        if (m_node->m_states.top() != nullptr)
                        {
                            m_node->get_state().recieve_callback(json);
                        }else {
                            m_node->print("No state to recieve message");
                        } });

        m_connected = true;
    }

    auto real_comms::update() -> void { mesh.update(); }
    auto real_comms::get_ID() -> int { return mesh.getNodeId(); }

    auto real_comms::receive(int from_who, std::string message) -> void
    {
        DynamicJsonDocument jsonBuffer(1024 + message.length());
        DeserializationError error = deserializeJson(jsonBuffer, message);

        if (error)
        {
            node_ptr.get()->print("deserializeJson() failed: " + std::string(error.c_str()));
            return;
        }

        std::cout << "Received: " << message << std::endl;

        // m_messages.push(jsonBuffer);
    }

    auto real_comms::send(int to, std::string message, std::string query) -> void
    {
        String json;

        DynamicJsonDocument jsonBuffer(1024);
        JsonObject message_body = jsonBuffer.to<JsonObject>();

        message_body["body"] = message;
        message_body["from"] = node_ptr.get()->get_information().get_ID();
        message_body["where"] = node_ptr.get()->get_information().get_position();
        message_body["query"] = query;

        serializeJson(message_body, json);
        mesh.sendSingle(to, json);
    }

    auto real_comms::send_broadcast(std::string message, std::string query) -> void
    {
        String json;

        DynamicJsonDocument jsonBuffer(1024);
        JsonObject message_body = jsonBuffer.to<JsonObject>();

        message_body["body"] = message;
        message_body["from"] = node_ptr.get()->get_information().get_ID();
        message_body["where"] = node_ptr.get()->get_information().get_position();
        message_body["query"] = query;

        serializeJson(message_body, json);
        mesh.sendBroadcast(json);
    }

    auto real_comms::parse_message(String &message) -> JsonObject
    {
        DynamicJsonDocument jsonBuffer(1024 + message.length());
        DeserializationError error = deserializeJson(jsonBuffer, message);

        if (error)
        {
            node_ptr.get()->print("deserializeJson() failed: " + std::string(error.c_str()));
            return jsonBuffer.as<JsonObject>();
        }

        return jsonBuffer.as<JsonObject>();
    }

    auto real_comms::make_json(std::string message_body, std::string query) -> JsonObject
    {
        DynamicJsonDocument jsonBuffer(1024);
        JsonObject message = jsonBuffer.to<JsonObject>();

        message["body"] = message_body;
        message["from"] = node_ptr.get()->get_information().get_ID();
        message["where"] = node_ptr.get()->get_information().get_position();
        message["query"] = query;

        return message;
    }

#else

    auto fake_comms::send() -> void {}
    auto fake_comms::receive() -> void {}
    auto fake_comms::update() -> void {}

#endif
}