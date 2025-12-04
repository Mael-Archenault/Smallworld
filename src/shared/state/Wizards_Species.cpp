#include "Tribe.h"
#include "Wizards_Species.h"

using namespace state;

Wizards_Species::Wizards_Species(): Species_Description("Wizards",5,10) {
    effect_disabled = false;
}

int Wizards_Species::get_bonus_rewards(std::vector<Area*> areas){
    int bonus_reward = 0;
    if(effect_disabled){
        return 0;
    }
    for(Area* area : areas){
        for(Area_Specialization area_specialization : area->get_area_specialization()){
            if(area_specialization == MAGIC_REGION){
                bonus_reward++;
            }
        }
    }
    return bonus_reward;
}

void Wizards_Species::disable_effects(std::vector<Area*> areas){
    effect_disabled = true;
}