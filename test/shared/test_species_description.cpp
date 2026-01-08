#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

class Species_Description_Observer : public effects::Species_Description
{
   public:
    Species_Description_Observer(std::string name, int given_units_number, int max_units_number)
        : effects::Species_Description(name, given_units_number, max_units_number) {};

    int get_max_units_number()
    {
        return max_units_number;
    }
};

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestSpeciesDescription)
{
    {
        // testing initialization

        Species_Description_Observer species_description("TestSpecies", 5, 10);
        BOOST_CHECK_EQUAL(species_description.get_name(), "TestSpecies");
        BOOST_CHECK_EQUAL(species_description.get_initial_units_number(), 5);
        BOOST_CHECK_EQUAL(species_description.get_max_units_number(), 10);

        // testing methods

        BOOST_CHECK_EQUAL(species_description.get_initial_units_number(), 5);
        std::vector<state::Area_Specialization> area_specs;
        state::Area               area(0, 10, state::Area_Biome::HILL, area_specs, false);
        std::vector<state::Area*> areas;
        areas.push_back(&area);
        // bonus reward not implemented yet so it should return 0
        BOOST_CHECK_EQUAL(species_description.rewards_effect(areas), 0);

        BOOST_CHECK_EQUAL(species_description.get_name(), "TestSpecies");
    }

    {
    }
}
