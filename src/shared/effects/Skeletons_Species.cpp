
#include "effects/Skeletons_Species.h"

#include "state/Area.h"

namespace effects
{

Skeletons_Species::Skeletons_Species() : Species_Description("Skeletons", 6, 20)
{
    max_units_number          = 20;
    non_empty_areas_conquered = 0;
}

int Skeletons_Species::second_gather_effect(int initial_free_units_number)
{
    int additional_free_units = non_empty_areas_conquered / 2;
    non_empty_areas_conquered = 0;
    return std::min(initial_free_units_number + additional_free_units, max_units_number);
}

void Skeletons_Species::conquest_effect(state::Area* attacked_area)
{
    if (attacked_area->get_units_number() != 0)
    {
        non_empty_areas_conquered++;
    }
}
}  // namespace effects