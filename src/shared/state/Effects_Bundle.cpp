//
// Created by vboxuser on 10/17/25.
//

#include "Effects_Bundle.h"

using namespace state;

Effects_Bundle::Effects_Bundle() = default;

void Effects_Bundle::apply_first_round_effect (){}

void Effects_Bundle::apply_conquest_effect (Area* area){}

void Effects_Bundle::apply_lose_effect (){}

int Effects_Bundle::get_bonus_rewards (std::vector<Area*> areas){
    return 0;
}

void Effects_Bundle::apply_additional_defense(Area* areas){}

int Effects_Bundle::add_free_units(int current_units_number){
    return 0;
}

void Effects_Bundle::disable_effects(){}

void Effects_Bundle::on_abandon_area(Area* area){}

bool Effects_Bundle::is_area_impenetrable(Area* area){
    return false;
}