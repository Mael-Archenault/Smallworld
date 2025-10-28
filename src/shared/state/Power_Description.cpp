#include "Power_Description.h"

namespace state
{

    Power_Description::Power_Description(std::string name, int given_units_number, Effects_Bundle effects) : name(name), given_units_number(given_units_number), effects(effects) {}

    int Power_Description::get_bonus_rewards(Area &area)
    {
        return 0;
    }

    int Power_Description::get_initial_units_number()
    {
        return given_units_number;
    }

    std::string Power_Description::get_name()
    {
        return name;
    }

}