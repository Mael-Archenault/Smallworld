#include "Tribe.h"
#include "Area_Biome.h"

#include <stdexcept>
#include <unordered_set>
#include <algorithm>

#include "Game_State.h"
#include "Map.h"

namespace state {

Tribe::Tribe(int id,Species_Description* species_description, Power_Description* power_description): id(id), species_description(species_description), power_description(power_description) {
    owned_areas = std::vector<Area*>();
    in_decline = false;
    free_units_number = species_description->get_initial_units_number() + power_description->get_initial_units_number();
}
Species_Description* Tribe::get_species_description() {
    return species_description;
}

Power_Description* Tribe::get_power_description() {
    return power_description;
}

void Tribe::gather_free_units() {
    for (size_t i=0; i<owned_areas.size(); i++) {
        // Remove all units except one from each area and add them to free_units_number
        free_units_number = free_units_number + owned_areas[i]->gather_free_units();
    }
}

int Tribe::get_free_units_number() {
    return free_units_number;
}


std::vector<std::pair<int, int>> Tribe::get_conquest_prices(Map* map) {
    if (owned_areas.size() == 0) {
        return map->get_starting_points_prices(*this,false /*species_description.can_start_anywhere();*/);
    }

    std::vector<std::pair<int, int>> prices;
    std::unordered_set<int> seen;
    prices.reserve(owned_areas.size() * 3);

    for (Area* area : owned_areas) {
        if (!area) continue;
        for (Area* neighbor : area->get_neighbors()) {
            if (!neighbor) continue;
            if (neighbor->get_owner_tribe() == this) continue;
            int nid = neighbor->id;
            if (seen.find(nid) != seen.end()) continue;
            seen.insert(nid);
            int price = neighbor->get_conquest_price(*this);
            prices.push_back(std::pair<int, int>{nid, price});
        }
    }
    return prices;
}

void Tribe::redeploy_units(int area_id, int n_added_units) {
    if (n_added_units > free_units_number){
        throw std::invalid_argument("redeploy_units: not enough free units to redeploy.");
    }
    // Find the area by ID
    for (size_t i=0; i<owned_areas.size(); i++) {
        if (owned_areas[i]->id == area_id) {
            owned_areas[i]->deploy_units(n_added_units);
            free_units_number -= n_added_units;
            return;
        }
    }
    throw std::invalid_argument("redeploy_units: area_id not found in owned_areas");
}

void Tribe::conquer(Area* attacked_area, int n_units, int dice_units) {
    if (n_units<attacked_area->get_conquest_price(*this)){
        throw std::invalid_argument("conquer: not enough units to conquer the area.");
    }
    if (n_units > free_units_number) {
        throw std::invalid_argument("conquer: not enough free units to conquer the area.");
    }
    attacked_area->set_owner_tribe(this);
    attacked_area->set_units_number(n_units);
    free_units_number-=n_units;
    owned_areas.push_back(attacked_area);
}

void Tribe::go_in_decline(){
    in_decline = true;
}
int Tribe::get_rewards(){
    return 0;
}

std::string Tribe::get_species_name(){
    return species_description->get_name();
}
std::string Tribe::get_power_name(){
    return power_description->get_name();
}

bool Tribe::is_in_decline(){
    return in_decline;
}


void Tribe::remove_from_map(){
    std::vector<Area*> copy(owned_areas);
    for (auto& area: copy){
        area->set_owner_tribe(nullptr);
        area->clear_units();
    }
}
void Tribe::remove_from_owned_areas(Area* area){
    if (std::find(owned_areas.begin(), owned_areas.end(), area) == owned_areas.end()) {
        throw std::invalid_argument("Tribe : remove_from_owned_areas: area not owned by the tribe");
    }
    owned_areas.erase(std::find(owned_areas.begin(), owned_areas.end(), area));
}

}
