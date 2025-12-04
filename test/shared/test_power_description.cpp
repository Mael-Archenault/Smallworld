#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestPowerDescription)
{
    {
        // testing initialization
        effects::Power_Description power_description("TestPower", 5);
        BOOST_CHECK_EQUAL(power_description.get_name(), "TestPower");
        BOOST_CHECK_EQUAL(power_description.get_initial_units_number(), 5);

        // testing methods

        BOOST_CHECK_EQUAL(power_description.get_initial_units_number(), 5);
        std::vector<state::Area_Specialization> area_specs;
        state::Area               area(0, 10, state::Area_Biome::HILL, area_specs, false);
        std::vector<state::Area*> areas;
        areas.push_back(&area);
        // bonus reward not implemented yet so it should return 0
        BOOST_CHECK_EQUAL(power_description.rewards_effect(areas), 0);

        BOOST_CHECK_EQUAL(power_description.get_name(), "TestPower");
    }

    {
    }
}