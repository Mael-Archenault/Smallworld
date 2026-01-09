#include "effects/Wealthy_Power.h"
#include "state/Area.h"

namespace effects
{
Wealthy_Power::Wealthy_Power() : Power_Description("Alchemists", 4)
{
    rich = false;
}

int Wealthy_Power::rewards_effect(std::vector<state::Area*> owned_areas)
{
    if(!rich)
    {
        rich = true;
        return 7;
    }
    return 0;
}
}