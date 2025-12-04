#include "effects/Dwarves_Species.h"

#include <iostream>

#include "state/Area.h"

namespace effects
{

Dwarves_Species::Dwarves_Species() : Species_Description("Dwarves", 3, 8) {}

int Dwarves_Species::rewards_effect(std::vector<state::Area*> owned_areas)
{
    int bonus_reward = 0;
    for (state::Area* area : owned_areas)
    {
        for (state::Area_Specialization area_specialization : area->get_area_specialization())
        {
            if (area_specialization == state::Area_Specialization::MINE)
            {
                bonus_reward++;
            }
        }
    }
    return bonus_reward;
}
}  // namespace effects