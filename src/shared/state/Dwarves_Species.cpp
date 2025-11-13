//
// Created by vboxuser on 10/17/25.
//
#include "Dwarves_Species.h"

using namespace state;

Dwarves_Species::Dwarves_Species(): Species_Description("Dwarves",3,8) {}

int Dwarves_Species::get_bonus_rewards(std::vector<Area*> areas){
    int bonus_reward = 0;
    for(Area* area : areas){
        for(Area_Specialization area_specialization : area->get_area_specialization()){
            if(area_specialization == MINE){
                bonus_reward++;
            }
        }
    }
    return bonus_reward;
}