#include "Tribe.h"
#include "Wizards_Species.h"

using namespace state;

Wizards_Species::Wizards_Species(): Species_Description("Wizards",5,10) {}

int Wizards_Species::get_bonus_rewards(Area* area){
    for(Area_Specialization area_specialization : area->get_area_specialization())
    if(area_specialization == MAGIC_REGION){
        return 1;
    }
    return 0;
}