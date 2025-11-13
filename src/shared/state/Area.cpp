#include "Area.h"
#include <random>
#include <stdexcept>
#include "Species_Description.h"
#include "Power_Description.h"

using namespace state;

const int default_attacking_units_required = 2;

Area::Area(int id, int n_units, Area_Biome biome, std::vector<Area_Specialization> area_specialization)
    : id(id),
      units_number(n_units),
      biome(biome),
      owner_tribe(nullptr),
      area_specialization(area_specialization)
{
    if (biome==state::Area_Biome::MOUNTAINS){
        special_tokens.push_back(state::Area_Special_Token::MOUNTAIN);
    }
    
}


int Area::gather_free_units() {
    
    if (units_number <= 1) 
        return 0;
    int to_return = units_number - 1;
    units_number = 1;
    return to_return;
}

int Area::get_conquest_price(Tribe& attacking_tribe) {
    return units_number + special_tokens.size() + default_attacking_units_required;
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

int Area::get_units_number() {
    return units_number;
}

Tribe* Area::get_owner_tribe() {
    if (owner_tribe == nullptr) {
        return new Tribe(-1, new Species_Description("Lost Tribe", 0, 0), new Power_Description("No Power", 0));
    }
    return owner_tribe;
}

std::vector<Area_Special_Token>& Area::get_special_tokens() {
    return special_tokens;
}

std::vector<Area_Specialization>& Area::get_area_specialization() {
    return area_specialization;
}


Area_Biome Area::get_biome(){
    return biome;
}

void Area::set_special_tokens(Area_Special_Token special_token){
    special_tokens.push_back(special_token);
}