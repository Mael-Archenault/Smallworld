#include "Tribe.h"

#include <stdexcept>

namespace state {

Tribe::Tribe(Species_Description* species_description, Power_Description* power_description): id(next_id), species_description(species_description), power_description(power_description) {
    next_id++;
    owned_areas = std::vector<Area*>();
    in_decline = false;
    free_units_number = species_description->get_initial_units_number() + power_description->get_initial_units_number();
}
int Tribe::next_id =0;
Species_Description* Tribe::get_species_description() {
    return species_description;
}

Power_Description* Tribe::get_power_description() {
    return power_description;
}

void Tribe::gather_free_units() {
    for (auto area : owned_areas) {
        // Remove all units except one from each area and add them to free_units_number
        free_units_number = free_units_number + area->gather_free_units();
    }
}

int Tribe::get_free_units_number() {
    return free_units_number;
}

std::vector<std::vector<int>> Tribe::get_conquest_prices() {
    std::vector<std::vector<int>> prices;
    for (auto area : owned_areas) {
        for (auto neighbor : area->get_neighbors()) {
            // if (neighbor->getOwner_tribe()!=this) {
                int price = neighbor->get_conquest_price(*this);
                std::vector<int> labelled_price;
                labelled_price.push_back(neighbor->id);
                labelled_price.push_back(price);
                prices.push_back(labelled_price);
            // }
        }
    }
    return prices;
}

void Tribe::redeploy_units(int area_id, int n_added_units) {
    if (n_added_units > free_units_number){
        throw std::invalid_argument("Error, sending more units than current free_units");
    }
    // Find the area by ID
    for (auto area : owned_areas) {
        if (area->id == area_id) {
            area->deploy_units(n_added_units);
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
        throw std::invalid_argument("Error, sending more units than current free_units");
        return;
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
}
