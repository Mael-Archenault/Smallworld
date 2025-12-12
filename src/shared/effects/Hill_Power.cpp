#include "effects/Hill_Power.h"

#include "state/Area.h"

namespace effects
{

Hill_Power::Hill_Power() : Power_Description("Hill", 4)
{
    effect_disabled = false;
}

int Hill_Power::rewards_effect(std::vector<state::Area*> owned_areas)
{
    int bonus_reward = 0;
    if (effect_disabled)
    {
        return 0;
    }
    for (state::Area* area : owned_areas)
    {
        if (area->get_biome() == state::Area_Biome::HILL)
        {
            bonus_reward++;
        }
    }
    return bonus_reward;
}

void Hill_Power::decline_effect(std::vector<state::Area*> areas)
{
    effect_disabled = true;
}
}  