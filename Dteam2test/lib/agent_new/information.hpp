#if !defined(INFORMATION_HPP_)
#define INFORMATION_HPP_

#include <memory>

#include "comms.hpp"

namespace dteam
{
    class information
    {
    public:
        information(){};
        information(std::shared_ptr<dteam::comms> comms_ptr) : ID(comms_ptr.get()->get_ID()), battery_level(rand() % 100), destination(rand() % 100), position(rand() % 100), cargo(rand() % 100){};
        ~information(){};

    public:
        int get_ID() const { return ID; };
        double get_battery_level() const { return battery_level; };
        int get_destination() const { return destination; };
        int get_cargo() const { return cargo; };
        int get_position() const { return position; };
        int get_fake_score() const { return rand() % 100; };

    public:
        double update_battery_level(double amount) { return battery_level = amount; };
        int update_destination(int new_destination) { return destination = new_destination; };
        int update_position(int new_position) { return position = new_position; };
        int update_cargo(int new_cargo) { return cargo = new_cargo; };

    private:
        int ID;
        double battery_level;

    private:
        int destination;
        int position;
        int cargo;
    };
}

#endif // INFORMATION_HPP_