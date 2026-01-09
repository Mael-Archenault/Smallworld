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
    state::Area               area(0, 5, state::Area_Biome::FARM, {}, false);
    owned_areas.push_back(&area);


    effects::Wealthy_Power wealthy_power;

    BOOST_CHECK_EQUAL(wealthy_power.rewards_effect(owned_areas),7);
    BOOST_CHECK_EQUAL(wealthy_power.rewards_effect(owned_areas),0);

}