#include "Tribe.h"
#include "Humans_Species.h"

using namespace state;

Humans_Species::Humans_Species(): Species_Description("Humans",5,10) {}

int Humans_Species::get_bonus_rewards(Area* area){
    if(area->get_biome() == FARM){
        return 1;
    }
    return 0;
}