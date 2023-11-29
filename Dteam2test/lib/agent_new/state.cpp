#include <iostream>

#include "state.hpp"
#include "node.hpp"
#include "comms.hpp"

#include "Wire.h"

namespace dteam
{
    // ============================================================
    // State
    // ============================================================

    auto state::recieve_callback(JsonObject json) -> void
    {
        if (json.containsKey("query"))
        {
            if (json["query"] == "ask")
            {
                if (json["body"] == "priority_score")
                {
                    // MAGIC NUMBER 5 REPLACE WITH PRIO SCORE
                    m_node->get_comms()->send(json["from"].as<int>(), "5", "answer priority_score");
                }
            }
        }
    }

    // ============================================================
    // Initializing State
    // ============================================================

    auto initializing_state::enter() -> void
    {
        m_node.get()->print("Initializing...");
        m_node.get()->get_comms()->initialize(m_node);
        m_node.get()->print("comms done");

        m_node.get()->set_information(dteam::information(m_node.get()->get_comms()));
        m_node.get()->print("information set");
        m_node->set_calculator(dteam::calculation(m_node));
        m_node.get()->print("calc set");

        //init oled shield

        //m_node->set_oled(dteam::oled(m_node));
        m_node.get()->get_oled()->initialize();

        m_node.get()->print("oled initiated");

    }

    auto initializing_state::execute() -> void
    {

        if (m_node.get()->get_oled()->Disp_ok())
        {
            m_node.get()->print("Node initialized, switching to idle state.");

            auto idle_state = std::unique_ptr<dteam::idle_state>(new dteam::idle_state(m_node));
            m_node.get()->set_state(std::move(idle_state));
            return;
        }

        m_node.get()->print("Comms not established, retrying...");
        enter();
    }

    auto initializing_state::exit() -> void { m_node.get()->print("Done initializing."); }

    // ============================================================
    // Idle State
    // ============================================================

    auto idle_state::enter() -> void 
    { 
        m_node.get()->print("Entering idle state."); 
        m_node.get()->get_oled()->performAction(2);


    }

    auto idle_state::execute() -> void
    {
        // Check for cargo and destination, if both are present switch to main mission state

        if (m_node.get()->get_information().get_cargo() > 0 && m_node.get()->get_information().get_destination() != 0)
        {
            m_node.get()->print("Cargo and destination present, switching to main mission state.");

            auto main_mission_state = std::unique_ptr<dteam::main_mission_state>(new dteam::main_mission_state(m_node));
            m_node.get()->set_state(std::move(main_mission_state));

            return;
        }
    }

    auto idle_state::exit() -> void { m_node.get()->print("Exiting idle state."); }

    // ============================================================
    // Main Mission State
    // ============================================================

    auto main_mission_state::enter() -> void
    {
        // Checking if levels are ok. TODO: Implement beräkningsmodul to use here, for now we check if battery is equal to destination

        m_node.get()->print("Entering main mission state.");

        if (m_node.get()->get_information().get_battery_level() >= m_node.get()->get_information().get_destination())
        {
            m_levels_ok = true;
        }

        if (m_node.get()->get_information().get_cargo() == 0 && m_node.get()->get_information().get_destination() == 0)
        {
            m_transport_done = true;
        }
    }

    auto main_mission_state::execute() -> void
    {
        if (m_transport_done)
        {
            m_node.get()->pop_state();
            return;
        }

        if (m_levels_ok)
        {
            m_node.get()->print("Levels ok, switching to transport state.");

            auto transport_state = std::unique_ptr<dteam::transport_state>(new dteam::transport_state(m_node));
            m_node.get()->set_state(std::move(transport_state));

            return;
        }

        m_node.get()->print("Levels not ok, switching to charging mission state.");

        auto charging_mission_state = std::unique_ptr<dteam::charging_mission_state>(new dteam::charging_mission_state(m_node));
        m_node.get()->set_state(std::move(charging_mission_state));
    }

    auto main_mission_state::exit() -> void { m_node.get()->print("Exiting main mission state."); }

    // ============================================================
    // Transport State
    // ============================================================

    auto transport_state::enter() -> void
    {
        m_node.get()->print("Entering transport state.");


        

        // my position, my destination, updates continously to visulice moving
        m_node.get()->get_oled()->performAction(4);
        // TESTING REMOVE LATER

        // auto message = m_node.get()->get_comms().get()->make_json("priority_score", "information");
        // m_node.get()->get_comms().get()->send_broadcast(message);
    }

    auto transport_state::execute() -> void
    {
        if (m_node.get()->get_information().get_position() == m_node.get()->get_information().get_destination())
        {
            m_node.get()->print("Destination reached.");

            m_node.get()->get_information().update_cargo(0);
            m_node.get()->get_information().update_destination(0);
            m_node.get()->get_oled()->performAction(4);

            m_node.get()->pop_state();

            return;
        }

        m_node.get()->print("Moving towards destination, current position: " + std::to_string(m_node.get()->get_information().get_position()) +
                            ", destination: " + std::to_string(m_node.get()->get_information().get_destination()));

        // TODO: Implement movement here, for now we just update the position and battery level

        int speed = m_node.get()->get_information().get_destination() > m_node.get()->get_information().get_position() ? 1 : -1;

        m_node.get()->get_information().update_position(m_node.get()->get_information().get_position() + speed);
        m_node.get()->get_information().update_battery_level(m_node.get()->get_information().get_battery_level() - abs(speed));
    }

    auto transport_state::exit() -> void { m_node.get()->print("Exiting transport state."); }

    // ============================================================
    // Charging Mission State
    // ============================================================

    auto charging_mission_state::enter() -> void
    {
        m_node.get()->print("Entering charging mission state, getting charging station position.");
        m_node.get()->get_oled()->performAction(5);


       
       
        // TODO: Implement charging station position here, for now we just set it to...64 

        //Move to charher (coordinates to charger), Current battery, Desired battery


        m_charging_station_position = 45;
    }

    auto charging_mission_state::execute() -> void
    {
        m_node.get()->get_oled()->performAction(5);

        if (m_node.get()->get_information().get_battery_level() >= m_node.get()->get_information().get_destination())
        {
            m_node.get()->pop_state();
            return;
        }

        if (m_node.get()->get_information().get_position() == m_charging_station_position)
        {
            m_node.get()->print("Charging station reached, switching to arrival state.");

            auto arrival_state = std::unique_ptr<dteam::arrival_state>(new dteam::arrival_state(m_node));
            m_node.get()->set_state(std::move(arrival_state));

            return;
        }

        // TODO: Implement movement here, for now we just update the position and battery level

        int speed = m_charging_station_position > 0 ? 1 : -1;

        m_node.get()->get_information().update_position(m_node.get()->get_information().get_position() + speed);
        m_node.get()->get_information().update_battery_level(m_node.get()->get_information().get_battery_level() - abs(speed));

        m_node.get()->print("Moving towards charging station, current position: " + std::to_string(m_node.get()->get_information().get_position()) +
                            ", charging station position: " + std::to_string(m_charging_station_position));
    }

    auto charging_mission_state::exit() -> void { m_node.get()->print("Exiting charging mission state."); }

    // ============================================================
    // Arrival State
    // ============================================================

    auto arrival_state::recieve_callback(JsonObject root) -> void
    {
        if (root.containsKey("query")) // TODO: ADD PREPROCESSING FOR ALL STATES BEFORE CALLING SPECIFIC STATE CALLBACKS
        {
            if (root["query"].as<std::string>() == "ask")
            {
                if (root["body"].as<std::string>() == "priority_score")
                {
                    // MAGIC NUMBER 5 REPLACE WITH PRIO SCORE
                    m_node->print("Recieved priority score request, sending priority score.");
                    m_node->get_comms()->send(root["from"].as<int>(), "5", "answer priority_score");
                }
            }
        }

        if (root["query"].as<std::string>() == "answer priority_score")
        {
            m_node->print(root["from"].as<std::string>() + " sent priority score, adding it to map.");
            m_node_scores.insert({root["from"].as<int>(), root["body"].as<int>()});

            // Check if we have everyone
            auto node_count = 0;
            auto node_list = m_node->get_mesh().getNodeList();

            for (auto &node : node_list)
            {
                if (m_node->get_mesh().isConnected(node))
                    node_count++;
            }

            if (m_node_scores.size() == node_count)
                m_ready_to_send_queue_numbers = true;
        }
    }

    auto arrival_state::enter() -> void
    {
        m_node.get()->get_oled()->performAction(6);

        m_node.get()->get_comms().get()->send_broadcast("priority_score", "ask");

        if (m_node.get()->get_information().get_battery_level() >= m_node.get()->get_information().get_destination())
        {
            m_node.get()->pop_state();
            return;
        }
    }

    auto arrival_state::execute() -> void
    {
        if (m_ready_to_send_queue_numbers)
        {
            auto fake_queue_ticket = 0;

            for (auto &node : m_node_scores)
            {
                m_node->get_comms()->send(node.first, std::to_string(fake_queue_ticket), "queue_ticket");
                fake_queue_ticket++;
            }

            m_node->queue_ticket = fake_queue_ticket;

            auto queue_state = std::unique_ptr<dteam::queue_state>(new dteam::queue_state(m_node));
            m_node.get()->set_state(std::move(queue_state));
        }
    }

    auto arrival_state::exit() -> void { m_node.get()->print("Exiting arrival state."); }

    // ============================================================
    // Queue State
    // ============================================================

    auto queue_state::enter() -> void
    {
        m_node.get()->print("Entering queue state.");

        m_node.get()->get_oled()->performAction(7);


        if (m_node.get()->get_information().get_battery_level() >= m_node.get()->get_information().get_destination())
        {
            m_node.get()->pop_state();
            return;
        }
    }

    auto queue_state::execute() -> void
    {
        m_node.get()->get_oled()->performAction(7);

        if (m_node->queue_ticket == 0)
        {
            m_node->print("Queue ticket is 0, switching to charging state.");

            auto charging_state = std::unique_ptr<dteam::charging_state>(new dteam::charging_state(m_node));
            m_node.get()->set_state(std::move(charging_state));

            return;
        }

        m_node->print("Queue ticket is " + std::to_string(m_node->queue_ticket) + ", waiting for queue to be empty.");
    }

    auto queue_state::exit() -> void { m_node.get()->print("Exiting queue state."); }

    auto queue_state::recieve_callback(JsonObject root) -> void
    {
        if (root["query"].as<std::string>() == "queue_ticket")
        {
            m_node->print("Recieved queue ticket, saving.");
            m_node->queue_ticket = root["body"].as<int>();
        }

        if (root.containsKey("query")) // TODO: ADD PREPROCESSING FOR ALL STATES BEFORE CALLING SPECIFIC STATE CALLBACKS
        {
            if (root["query"].as<std::string>() == "ask")
            {
                if (root["body"].as<std::string>() == "priority_score")
                {
                    // MAGIC NUMBER 5 REPLACE WITH PRIO SCORE
                    m_node->print("Recieved priority score request, sending priority score.");
                    m_node->get_comms()->send(root["from"].as<int>(), "5", "answer priority_score");
                }
            }
        }

        // if (root["query"].as<std::string>() == "answer priority_score")
        // {
        //     m_node->print(root["from"].as<std::string>() + " sent priority score, adding it to map.");
        //     m_node_scores.insert({root["from"].as<int>(), root["body"].as<int>()});

        //     // Check if we have everyone
        //     auto node_count = 0;
        //     auto node_list = m_node->get_mesh().getNodeList();

        //     for (auto &node : node_list)
        //     {
        //         if (m_node->get_mesh().isConnected(node))
        //             node_count++;
        //     }

        //     if (m_node_scores.size() == node_count)
        //         m_ready_to_send_queue_numbers = true;
        // }
    }

    // ============================================================
    // Charging State
    // ============================================================

    auto charging_state::enter() -> void 
    { 
        m_node.get()->print("Entering charging state."); 

        m_node.get()->get_oled()->performAction(8);

    
    }

    auto charging_state::execute() -> void
    {
        m_node.get()->get_oled()->performAction(8);

        if (m_node.get()->get_information().get_battery_level() >= m_node.get()->get_information().get_destination())
        {
            m_node.get()->pop_state();
            return;
        }

        m_node.get()->print("Charging, current battery level: " + std::to_string(m_node.get()->get_information().get_battery_level()) +
                            ", destination: " + std::to_string(m_node.get()->get_information().get_destination()));
    }

    auto charging_state::exit() -> void { m_node.get()->print("Exiting charging state."); }
};