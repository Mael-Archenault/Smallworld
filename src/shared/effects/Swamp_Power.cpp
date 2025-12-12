#include "effects/Swamp_Power.h"

#include "state/Area.h"

namespace effects
{

Swamp_Power::Swamp_Power() : Power_Description("Swamp", 4)
{
    effect_disabled = false;
}

int Swamp_Power::rewards_effect(std::vector<state::Area*> owned_areas)
{
    int bonus_reward = 0;
    if (effect_disabled)
    {
        return 0;
    }
    for (state::Area* area : owned_areas)
    {
        if (area->get_biome() == state::Area_Biome::SWAMP)
        {
            bonus_reward++;
        }
    }
    return bonus_reward;
}

void Swamp_Power::decline_effect(std::vector<state::Area*> areas)
{
    effect_disabled = true;
}
}  