#if !defined(CALCULATION_HPP)
#define CALCULATION_HPP

#include <algorithm>

#include "information.hpp"

namespace dteam
{   
    // class node;

    class calculation
    {

    protected:
    std::shared_ptr<dteam::node> m_node;

    public:
        calculation(){};
        calculation(std::shared_ptr<dteam::node> node) : m_node(node){};
        virtual ~calculation() = default;

    public:
        
        double get_priority_score();
        double get_battry_range();
        double get_battery_depletion();

    };


}


#endif // CALCULATION_H