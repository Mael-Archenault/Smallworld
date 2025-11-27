#include "Tribe.h"
#include "Humans_Species.h"

using namespace state;

Humans_Species::Humans_Species(): Species_Description("Humans",5,10) {}

int Humans_Species::get_bonus_rewards(std::vector<Area*> areas){
    int bonus_reward = 0;
    for(Area* area : areas){
        if(area->get_biome() == FARM){
                bonus_reward++;
        }
    }
    return bonus_reward;
}