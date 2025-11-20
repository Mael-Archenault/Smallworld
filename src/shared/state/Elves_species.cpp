#include "Elves_Species.h"
#include "Area.h"
#include "Tribe.h"

using namespace state;

Elves_Species:: Elves_Species(): Species_Description("Elves",6,8) {}

void Elves_Species::apply_lose_effect(Area* area) {
    if (!area || !area->get_owner_tribe()) return;

    Tribe* owner = area->get_owner_tribe();

    if (owner->get_species_description()->get_name() == "Elves") {
        int redeployed_units = area->get_units_number();
        owner->add_pending_redeploy_units(redeployed_units);
        area->set_units_number(0);
    }
}





