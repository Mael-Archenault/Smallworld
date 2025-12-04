#include "effects/Species_Description.h"

namespace effects
{

Species_Description::Species_Description(std::string name, int given_units_number,
                                         int max_units_number)
    : name(name), given_units_number(given_units_number), max_units_number(max_units_number)
{
}

int Species_Description::get_initial_units_number()
{
    return given_units_number;
}

std::string Species_Description::get_name()
{
    return name;
}
}  // namespace effects