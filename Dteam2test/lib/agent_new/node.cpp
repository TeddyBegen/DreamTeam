#include "node.hpp"

#include <iostream>

namespace dteam
{
    auto node::initialize(std::shared_ptr<dteam::comms> comms_ptr) -> void
    {
        m_comms = comms_ptr;

        auto state = new dteam::initializing_state(shared_from_this());
        this->set_state(std::unique_ptr<dteam::initializing_state>(state));
    }

    auto node::get_information() -> information & { return this->m_info; }
    auto node::set_information(const information &info) -> void { this->m_info = info; }

    auto node::get_state() -> dteam::state & { return *this->m_states.top(); }
    auto node::set_state(std::unique_ptr<dteam::state> state) -> void

    {
        if (!this->m_states.empty())
            this->m_states.top()->exit();

        m_states.push(std::move(state));
        m_states.top()->enter();
    }

    auto node::pop_state() -> void
    {
        if (this->m_states.empty())
            return;

        this->m_states.top()->exit();
        this->m_states.pop();

        if (!this->m_states.empty())
            this->m_states.top()->enter();
    }
};