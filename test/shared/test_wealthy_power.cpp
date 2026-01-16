#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Wealthy_Power)
{
    std::vector<state::Area*> owned_areas;
    state::Area               area1(0, 3, state::Area_Biome::MOUNTAINS,{}, false);
    state::Area               area2(1, 3, state::Area_Biome::HILL, {}, false);

    owned_areas.push_back(&area1);
    owned_areas.push_back(&area2);

    effects::Wealthy_Power wealthy_power;

    BOOST_CHECK_EQUAL(wealthy_power.rewards_effect(owned_areas), 7);

    BOOST_CHECK_EQUAL(wealthy_power.rewards_effect(owned_areas), 0); //2nd turn
}