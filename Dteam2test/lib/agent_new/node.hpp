#if !defined(NODE_HPP_)
#define NODE_HPP_

#include <memory>
#include <stack>

#include "information.hpp"
#include "state.hpp"
#include "comms.hpp"
#include "oled.hpp"
#include "calculation.hpp"

namespace dteam
{
    class node : public std::enable_shared_from_this<dteam::node>
    {
    public:
        node() = default;
        ~node() = default;

    public:
        auto initialize(std::shared_ptr<dteam::comms> comms_ptr) -> void;

    public:
        auto set_information(const dteam::information &info) -> void;
        auto get_information() -> dteam::information &;

        auto get_calculator() -> dteam::calculation & { return m_calc; }
        auto set_calculator(const dteam::calculation &calc) -> void { m_calc = calc; }

        auto get_state() -> dteam::state &;
        auto set_state(std::unique_ptr<dteam::state> state) -> void;
        auto pop_state() -> void;

        auto get_comms() -> std::shared_ptr<dteam::comms> { return m_comms; }
        // auto recieve_callback(String &msg) -> void
        // {
        //     Serial.println(msg.c_str());
        //     m_states.top()->recieve_callback(msg);
        // }

        auto get_oled() -> std::shared_ptr<dteam::oled> { return m_oled;}
        //auto set_oled(const dteam::oled &oled) ->void {}


#ifdef ARDUINO
        auto get_mesh() -> painlessMesh & { return mesh; }
#endif

    public:
        void print(std::string message) { std::cout << "ID:\t\t" << m_info.get_ID() << "\n"
                                                    << "Current State:\t" << m_states.top().get()->to_string() << "\n"
                                                    << "Printing:\t" << message << "\n\n"
                                                    << std::endl; }

        std::stack<std::unique_ptr<dteam::state>> m_states;
        
        // TEMP

        int queue_ticket = 0;

    private:
        dteam::information m_info;
        dteam::calculation m_calc;
        std::shared_ptr<dteam::comms> m_comms;
        std::shared_ptr<dteam::oled> m_oled;
    

    };
}

#endif // NODE_HPP_