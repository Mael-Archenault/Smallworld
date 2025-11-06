#include "Tribe.h"
#include "Trolls_Species.h"

using namespace state;

Trolls_Species::Trolls_Species(): Species_Description("Trolls",5,10){}

void Trolls_Species::apply_additional_defense(Area* area){
    area->set_special_tokens(TROLL_LAIR);
    return;
}