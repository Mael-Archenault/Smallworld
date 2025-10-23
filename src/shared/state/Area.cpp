#include "Area.h"

namespace state {

Area::Area(int n_units, Area_Biome biome, std::vector<Area_Specialization> area_specialization)
    : units_number(n_units),
      biome(biome),
      owner_tribe(nullptr),
      area_specialization(area_specialization),
      id(next_id)
{
    next_id++;
    if (biome==state::Area_Biome::MOUNTAINS){
        special_tokens.push_back(state::Area_Special_Token::MOUNTAIN);
    }
    
}
int Area::next_id =0;


int Area::gather_free_units() {
    
    if (units_number <= 1) 
        return 0;
    int to_return = units_number - 1;
    units_number = 1;
    return to_return;
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

void Area::add_neighbor(Area* neighbor) {
    neighbors.push_back(neighbor);
}
void Area::add_special_token(Area_Special_Token token) {
    special_tokens.push_back(token);
}

void Area::remove_special_token() {

    special_tokens.clear();
    special_tokens.push_back(Area_Special_Token::MOUNTAIN);
}


}

