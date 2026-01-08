
#include "effects/Humans_Species.h"

#include "state/Area.h"

namespace effects
{

Humans_Species::Humans_Species() : Species_Description("Humans", 5, 10)
{
    effect_disabled = false;
}

int Humans_Species::rewards_effect(std::vector<state::Area*> owned_areas)
{
    int bonus_reward = 0;
    if (effect_disabled)
    {
        return 0;
    }
    for (state::Area* area : owned_areas)
    {
        if (area->get_biome() == state::Area_Biome::FARM)
        {
            bonus_reward++;
        }
    }
    return bonus_reward;
}

void Humans_Species::decline_effect(std::vector<state::Area*> areas)
{
    effect_disabled = true;
}
}  // namespace effects