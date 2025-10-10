#include "Tribe.h"

namespace state {

Tribe::Tribe(){

}

Species_Description Tribe::get_species_description() {
}

Power_Description Tribe::get_power_description() {
}

void Tribe::gather_free_units() {
}

int Tribe::get_free_units_number() {
}

std::vector<int> Tribe::get_conquest_prices() {
}

void Tribe::roll_dice_for_bonus_units() {
}

void Tribe::deploy_units(int area_id, int n_added_units) {
}

void Tribe::conquer(Area& attacked_area, int n_units) {
}

}