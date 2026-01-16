#include "effects.h"
#include <algorithm>
#include "state.h"

namespace effects
{
Wealthy_Power::Wealthy_Power() : Power_Description("Wealthy", 4)
{
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