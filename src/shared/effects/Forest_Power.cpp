#include "effects/Forest_Power.h"

#include "state/Area.h"

namespace effects
{

Forest_Power::Forest_Power() : Power_Description("Forest", 4)
{
    effect_disabled = false;
}

int Forest_Power::rewards_effect(std::vector<state::Area*> owned_areas)
{
    int bonus_reward = 0;
    if (effect_disabled)
    {
        return 0;
    }
    for (state::Area* area : owned_areas)
    {
        if (area->get_biome() == state::Area_Biome::FOREST)
        {
            bonus_reward++;
        }
    }
    return bonus_reward;
}

void Forest_Power::decline_effect(std::vector<state::Area*> areas)
{
    effect_disabled = true;
}
}  