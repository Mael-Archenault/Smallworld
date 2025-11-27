#include "Trolls_Species.h"

#include "Tribe.h"

using namespace state;

Trolls_Species::Trolls_Species() : Species_Description("Trolls", 5, 10) {}

void Trolls_Species::apply_additional_defense(Area* area)
{
    for (Area_Special_Token specials_tokens : area->get_special_tokens())
    {
        if (specials_tokens == TROLL_LAIR)
        {
            return;
        }
    }
    area->set_special_tokens(TROLL_LAIR);
}

void Trolls_Species::disable_effects(std::vector<Area*> areas)
{
    for (Area* area : areas)
    {
        area->get_special_tokens().erase(std::remove(area->get_special_tokens().begin(),
                                                     area->get_special_tokens().end(), TROLL_LAIR),
                                         area->get_special_tokens().end());
    }
}