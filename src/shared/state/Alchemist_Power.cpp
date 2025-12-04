#include "Alchemist_Power.h"
#include "Tribe.h"

using namespace state;

Alchemist_Power::Alchemist_Power(): Power_Description("Alchemists",4){
    effect_disabled = false;
}

int Alchemist_Power::get_bonus_rewards(Area* area){
    if(effect_disabled){
        return 0;
    }
    return 2;
}

void Alchemist_Power::disable_effects(std::vector<Area*> areas){
    effect_disabled = true;
}