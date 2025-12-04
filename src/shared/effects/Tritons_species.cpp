#include "effects/Tritons_Species.h"

#include "state/Area.h"

namespace effects
{
Tritons_Species::Tritons_Species() : Species_Description("Tritons", 6, 8) {}

void Tritons_Species::apply_conquest_effect(state::Area* area)
{
    // if (!area) return;

    // Tribe* tribe = area->get_owner_tribe();
    // if (!tribe) return;

    // bool has_adjacent_water = false;

    // for (Area* neighbor : area->get_neighbors())
    // {
    //     if (!neighbor) continue;
    //     if (neighbor->get_biome() == Area_Biome::WATER && neighbor->get_owner_tribe() == tribe)
    //     {
    //         has_adjacent_water = true;
    //         break;
    //     }
    // }

    // int base_price  = area->get_conquest_price(*tribe);
    // int final_price = has_adjacent_water ? std::max(1, base_price - 1) : base_price;

    // if (tribe->get_free_units_number() >= final_price)
    // {
    //     tribe->conquer(area, final_price, 0);
    // }
}
}  // namespace effects