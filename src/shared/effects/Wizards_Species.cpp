#include "effects/Wizards_Species.h"

#include "state/Area.h"
using namespace effects;

Wizards_Species::Wizards_Species() : Species_Description("Wizards", 5, 10)
{
    effect_disabled = false;
}

int Wizards_Species::rewards_effect(std::vector<state::Area*> areas)
{
    int bonus_reward = 0;
    if (effect_disabled)
    {
        return 0;
    }
    for (state::Area* area : areas)
    {
        for (state::Area_Specialization area_specialization : area->get_area_specialization())
        {
            if (area_specialization == state::Area_Specialization::MAGIC_REGION)
            {
                bonus_reward++;
            }
        }
    }
    return bonus_reward;
}

void Wizards_Species::decline_effect(std::vector<state::Area*> areas)
{
    effect_disabled = true;
}