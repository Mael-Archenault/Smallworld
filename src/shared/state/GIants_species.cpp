//
// Created by vboxuser on 10/17/25.
//

#include "Area.h"
#include "Tribe.h"
#include "Giants_Species.h"
#include <algorithm>

namespace state {

Giants_Species::Giants_Species():Species_Description("Giants",6,8,Effects_Bundle()){}


void Giants_Species::apply_conquest_effect(Area* area) {
    Tribe* tribe = area->get_owner_tribe();
    if (!tribe) return;

    
    bool has_adjacent_mountain = (area->get_biome() == Area_Biome::MOUNTAINS && area->get_owner_tribe() == tribe);

    
    for (Area* neighbor : area->get_neighbors()) {
        if (!neighbor) continue;
        if (neighbor->get_biome() == Area_Biome::MOUNTAINS &&
            neighbor->get_owner_tribe() == tribe) {
            has_adjacent_mountain = true;
            break;
        }
    }

    
    int base_price = area->get_conquest_price(*tribe);
    int final_price = has_adjacent_mountain ? std::max(1, base_price - 1) : base_price;

    
    if (tribe->get_free_units_number() >= final_price) {
        tribe->conquer(area, final_price, 0);
    }
}
}

