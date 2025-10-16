#include "Area.h"

namespace state {

Area::Area(int n_units, Area_Biome biome, std::vector<Area_Specialization> area_specialization)
    : id(next_id),                
      units_number(n_units),      
      biome(biome),       
      owner_tribe(nullptr),
      area_specialization(area_specialization)
{
    next_id++;
    if (biome==state::Area_Biome::MOUNTAIN){
        special_tokens.push_back(state::Area_Special_Token::MOUNTAIN);
    }
    
}



int Area::gather_free_units() {
    
    if (units_number <= 1) 
        return 0;
    units_number = 1;
    return units_number-1;
}

int Area::get_conquest_price(Tribe& attacking_tribe) {

    int price = 2 ; 
    price = price + special_tokens.size();
    price = price + units_number;
    
    return price;
}

    
    

void Area::deploy_units(int n_added_units) {
    units_number += n_added_units;
}

void Area::set_owner_tribe(Tribe* new_owner_tribe) {
    owner_tribe = new_owner_tribe;
}

void Area::set_units_number(int n_units) {
    units_number = n_units;
}

std::vector<Area*> Area::get_neighbors() {
    return neighbors;
}

}