#include "Species_Description.h"

namespace state
{

    Species_Description::Species_Description(std::string name, int given_units_number, int max_units_number, Effects_Bundle effects) : name(name), given_units_number(given_units_number), max_units_number(max_units_number), effects(effects) {}

    int Species_Description::get_bonus_rewards(const Area &area)
    {
        return 0;
    }

    int Species_Description::get_initial_units_number()
    {
        return given_units_number;
    }
}
