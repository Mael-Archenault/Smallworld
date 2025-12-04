#include "effects/Orcs_Species.h"

#include "state/Area.h"

namespace effects
{

Orcs_Species::Orcs_Species() : Species_Description("Orcs", 5, 10)
{
    non_empty_areas_conquered = 0;
}

int Orcs_Species::rewards_effect(std::vector<state::Area*> owned_areas)
{
    int bonus_conquest_reward = non_empty_areas_conquered;
    non_empty_areas_conquered = 0;
    return bonus_conquest_reward;
}

void Orcs_Species::conquest_effect(state::Area* attacked_area)
{
    if (attacked_area->get_units_number() != 0)
    {
        non_empty_areas_conquered++;
    }
}
}  // namespace effects