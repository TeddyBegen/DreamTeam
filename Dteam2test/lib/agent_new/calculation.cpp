#include "calculation.hpp"
#include "node.hpp"

namespace dteam 
{
    double calculation::get_priority_score() 
    {

        //a * distance - (c/ ( battery + 1) ) - b * cargo weight

        /*
        viktning för distance
        */
        double distance_weight = 1.6;
        double distance = abs(m_node->get_information().get_position() - m_node->get_information().get_destination());
        double weighted_distance = distance_weight * distance;

        double battery_weight = 100;
        double battery = m_node->get_information().get_battery_level() + 1;
        double weighted_battery = battery_weight/battery;

        double cargo_weight = 0.2;
        double cargo = m_node->get_information().get_cargo();
        double weighted_cargo = cargo_weight * cargo;

        return weighted_distance - weighted_battery - weighted_cargo;
        
    }

    double calculation::get_battry_range() 
    {
        //(basförbrukning + (viktfaktor * vikt)) * distansen”

        double base_depletion = 1;
        double cargo_factor = 0.04;
        double cargo = m_node->get_information().get_cargo();
        double distance = abs(m_node->get_information().get_position() - m_node->get_information().get_destination());

        double battery_range = base_depletion + (cargo_factor * cargo) * distance;

        return battery_range;
    }

    double calculation::get_battery_depletion()
    {
        double base_depletion = 1;
        double cargo_factor = 0.04;
        double cargo = m_node->get_information().get_cargo();
        double distance = abs(m_node->get_information().get_position() - m_node->get_information().get_destination());

        double battery_depletion = (base_depletion * (cargo/100));

        return battery_depletion;

    }
    
}