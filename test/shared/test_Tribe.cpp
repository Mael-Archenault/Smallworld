//
// Created by vboxuser on 10/10/25.
//

#include <iostream>

#include "state/Tribe.h"



class test_Tribe : public state::Tribe {
public:
    test_Tribe(state::Species_Description* species_description, state::Power_Description* power_description, std::vector<state::Area*> owned_areas) : Tribe(species_description, power_description) {
        this->owned_areas = owned_areas;
    }

    int test_gather_free_units() {
        gather_free_units();
        return free_units_number;
    }
    std::vector<std::vector<int>> test_get_conquest_prices() {
        return get_conquest_prices();
    }
};

int main() {
    bool test_result = true ;

    //Scenario 1
    state::Effects_Bundle species_effect = state::Effects_Bundle();
    state::Species_Description * species_description = new state::Species_Description("test_Species",5,10,species_effect);
    state::Effects_Bundle power_effect = state::Effects_Bundle();
    state::Power_Description * power_description = new state::Power_Description("test_Power",4,power_effect);

    std::vector<state::Area*> owned_areas;
    state::Area * area1 = new state::Area();      //TODO once Area.cpp is ready
    state::Area * area2 = new state::Area();
    owned_areas.push_back(area1); owned_areas.push_back(area2);
    test_Tribe test_tribe = test_Tribe(species_description,power_description, owned_areas);

    if (test_tribe.test_gather_free_units() != 1) {     //TODO change "1" to the correct value based on the inputs to the areas
        test_result = false;
    }

    std::vector<std::vector<int>> conquest_prices = test_tribe.test_get_conquest_prices();
    for (std::vector<int> i : conquest_prices) {
        //TODO test each conquest price against Area.get_conquest_price. (need to call map with area id)
    }






    if (test_result == true) {
        std::cout << "Tribe tests were successful !";
        return 0;
    }
        std::cout << "Tribe tests failed !";
        return 1;
}