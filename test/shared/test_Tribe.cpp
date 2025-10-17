//
// Created by vboxuser on 10/10/25.
//

#include <iostream>

#include "state/Tribe.h"
using namespace state;


class test_Tribe : public Tribe {
public:
    test_Tribe(Species_Description* species_description, Power_Description* power_description, std::vector<Area*> owned_areas) : Tribe(species_description, power_description) {
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




#include <boost/test/unit_test.hpp>

#include <state.h>


BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestExemple)
{
    Effects_Bundle species_effect = Dwarf_Effects_Bundle();
    Species_Description * species_description = new Species_Description("test_Species",5,10,species_effect);
    Effects_Bundle power_effect = (Effects_Bundle) Dwarf_Effects_Bundle();
    Power_Description * power_description = new Power_Description("test_Power",4,power_effect);

    std::vector<Area*> owned_areas;
    Area * area1 = new Area(3, Area_Biome::HILL,{});
    Area * area2 = new Area(3, Area_Biome::HILL,{});

    owned_areas.push_back(area1);
    test_Tribe test_tribe = test_Tribe(species_description,power_description, owned_areas);
    test_tribe.conquer(*area2,5,0);

    BOOST_CHECK_EQUAL(test_tribe.test_gather_free_units(),13);
    
}

/*
int main() {
    bool test_result = true ;

    //Scenario 1
    Effects_Bundle species_effect = Effects_Bundle();
    Species_Description * species_description = new Species_Description("test_Species",5,10,species_effect);
    Effects_Bundle power_effect = Effects_Bundle();
    Power_Description * power_description = new Power_Description("test_Power",4,power_effect);

    std::vector<Area*> owned_areas;
    Area * area1 = new Area();
    Area * area2 = new Area();
    owned_areas.push_back(area1); owned_areas.push_back(area2);
    test_Tribe test_tribe = test_Tribe(species_description,power_description, owned_areas);

    if (test_tribe.test_gather_free_units() != 1) {
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
}*/