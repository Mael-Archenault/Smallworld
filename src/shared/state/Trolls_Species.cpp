#include "Trolls_Species.h"

#include "Tribe.h"

using namespace state;

Trolls_Species::Trolls_Species() : Species_Description("Trolls", 5, 10) {}

void Trolls_Species::apply_additional_defense(Area* area)
{
    area->add_special_token(TROLL_LAIR);
    return;
}