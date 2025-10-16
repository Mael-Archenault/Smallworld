#include "Area.h"

namespace state {

Area::Area()
    : id(0),                
      units_number(0),      
      biome(Area_Biome::FARM),       
      owner_tribe(nullptr)
{
}



int Area::gather_free_units() {
    
    if (units_number <= 1) 
        return 0; 

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

Area_Biome Area::get_biome() const {
    return biome;
}

void Area::set_biome(Area_Biome new_biome) {
    biome = new_biome;
}

std::vector<Area_Specialization>& Area::get_specializations() const {
    return area_specialization;
}

void Area::set_specializations(const std::vector<Area_Specialization>& specs) {
    area_specialization = specs;
}

}
