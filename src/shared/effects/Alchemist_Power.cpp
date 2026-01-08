#include "effects/Alchemist_Power.h"

#include "state/Area.h"

namespace effects
{
Alchemist_Power::Alchemist_Power() : Power_Description("Alchemists", 4)
{
    effect_disabled = false;
}

int Alchemist_Power::get_bonus_rewards(state::Area* area)
{
    if (effect_disabled)
    {
        return 0;
    }
    return 2;
}

void Alchemist_Power::disable_effects(std::vector<state::Area*> areas)
{
    effect_disabled = true;
}
}  // namespace effects