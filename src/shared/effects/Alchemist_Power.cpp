#include "effects/Alchemist_Power.h"

#include "state/Area.h"

namespace effects
{
Alchemist_Power::Alchemist_Power() : Power_Description("Alchemist", 4)
{
    effect_disabled = false;
}

int Alchemist_Power::rewards_effect(std::vector<state::Area*> owned_areas)
{
    if (effect_disabled)
    {
        return 0;
    }
    return 2;
}

void Alchemist_Power::decline_effect(std::vector<state::Area*> owned_areas)
{
    effect_disabled = true;
}
}  // namespace effects