#include "Tribe.h"

#include <stdexcept>
#include <unordered_set>

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
    for (int i=0; i<owned_areas.size(); i++) {
        // Remove all units except one from each area and add them to free_units_number
        free_units_number = free_units_number + owned_areas[i]->gather_free_units();
    }
}

int Tribe::get_free_units_number() {
    return free_units_number;
}

std::vector<std::vector<int>> Tribe::get_conquest_prices() {
    std::vector<std::vector<int>> prices;
    std::unordered_set<int> seen;
    prices.reserve(owned_areas.size() * 3);

    for (Area* area : owned_areas) {
        if (!area) continue;
        for (Area* neighbor : area->get_neighbors()) {
            if (!neighbor) continue;
            int nid = neighbor->id;
            if (seen.find(nid) != seen.end()) continue;
            seen.insert(nid);
            int price = neighbor->get_conquest_price(*this);
            prices.push_back(std::vector<int>{nid, price});
        }
    }
    return prices;
}

void Tribe::redeploy_units(int area_id, int n_added_units) {
    if (n_added_units > free_units_number){
        throw std::invalid_argument("Error, sending more units than current free_units");
    }
    // Find the area by ID
    for (int i=0; i<owned_areas.size(); i++) {
        if (owned_areas[i]->id == area_id) {
            owned_areas[i]->deploy_units(n_added_units);
            free_units_number -= n_added_units;
            return;
        }
    }
}

void Tribe::conquer(Area* attacked_area, int n_units, int dice_units) {
    if (n_units<attacked_area->get_conquest_price(*this)){
        throw std::invalid_argument("Error, not enough units to conquer this area");
    }
    if (n_units > free_units_number) {
        throw std::invalid_argument("Error, sending more units than current free units.");
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
}