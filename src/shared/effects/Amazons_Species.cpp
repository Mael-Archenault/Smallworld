#include "effects/Amazons_Species.h"

#include <iostream>

namespace effects
{
Amazons_Species::Amazons_Species() : Species_Description("Amazons", 6, 15) {}

int Amazons_Species::first_gather_effect(int initial_free_units_number)
{
    return std::min(initial_free_units_number + 4, max_units_number);
}

int Amazons_Species::second_gather_effect(int initial_free_units_number)
{
    return std::max(0, initial_free_units_number - 4);
}
}  // namespace effects