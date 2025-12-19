#include "effects.h"
#include "state/Area.h"

namespace effects
{

Pillaging_Power::Pillaging_Power() : Power_Description("Pillaging", 5)
{
    effect_disabled = false;
}

int Pillaging_Power::reward_effect(std::vector<state::Area*> owned_areas)
{
    
    if (effect_disabled)
    {
        return 0;
    }
    
    return pillaged_this_turn;
}

void Pillaging_Power::conquest_effect (state::Area* attacked_area){
    if (effect_disabled){
        return ;
    }
    
    if(attacked_area->get_units_number()!=0){
        pillaged_this_turn++;
    }
}
void Pillaging_Power::decline_effect(std::vector<state::Area*> areas)
{
    effect_disabled = true;
}
}  