#include "effects/Merchant_Power.h"

#include "state/Area.h"

namespace effects
{

Merchant_Power::Merchant_Power() : Power_Description("Merchant", 2)
{
    effect_disabled = false;
}

int Merchant_Power::rewards_effect(std::vector<state::Area*> owned_areas)
{
    int bonus_reward = 0;
    if (effect_disabled)
    {
        return 0;
    }
    for (state::Area* area : owned_areas)
    {
        
        bonus_reward++;
        
    }
    return bonus_reward;
}

void Merchant_Power::decline_effect(std::vector<state::Area*> areas)
{
    effect_disabled = true;
}
}  