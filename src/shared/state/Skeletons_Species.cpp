#include "Skeletons_Species.h"
#include "Tribe.h"

using namespace state;

Skeletons_Species::Skeletons_Species(): Species_Description("Skeletons",6,20){}

int Skeletons_Species::add_free_units(int current_units_number){
    int additional_free_units = non_empty_areas_conquered/2;
    non_empty_areas_conquered = 0;
    if(current_units_number+additional_free_units <= max_units_number){
        return additional_free_units;
    }
    if(max_units_number > current_units_number){
        return max_units_number - current_units_number;
    }
    return 0;
}

void Skeletons_Species::areas_conquered(Area* attacked_area){
    if(attacked_area->get_units_number() != 0){
        non_empty_areas_conquered++;
    }
}