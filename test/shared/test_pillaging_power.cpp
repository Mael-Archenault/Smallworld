#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Pillaging_Power)
{
    std::vector<state::Area*> owned_areas;
    
    state::Area               area1(0, 0, state::Area_Biome::FARM, {}, false);
    state::Area               area2(1, 3, state::Area_Biome::HILL, {}, false);
    state::Area               area3(2, 6, state::Area_Biome::HILL, {}, false);
    state::Area               area4(3, 0, state::Area_Biome::HILL, {}, false);

    owned_areas.push_back(&area1);
    owned_areas.push_back(&area2);
    owned_areas.push_back(&area3);
    owned_areas.push_back(&area4);

    effects::Pillaging_Power pillaging_power;

    
    BOOST_CHECK_EQUAL(pillaging_power.reward_effect(owned_areas), 0);

    pillaging_power.conquest_effect(&area1);
    BOOST_CHECK_EQUAL(pillaging_power.reward_effect(owned_areas), 0);

    pillaging_power.conquest_effect(&area1);
    pillaging_power.conquest_effect(&area2);
    BOOST_CHECK_EQUAL(pillaging_power.reward_effect(owned_areas), 1);

    pillaging_power.conquest_effect(&area1);
    pillaging_power.conquest_effect(&area2);
    pillaging_power.conquest_effect(&area3);
    BOOST_CHECK_EQUAL(pillaging_power.reward_effect(owned_areas), 2);

    pillaging_power.conquest_effect(&area1);
    pillaging_power.conquest_effect(&area2);
    pillaging_power.conquest_effect(&area3);
    pillaging_power.conquest_effect(&area4);
    BOOST_CHECK_EQUAL(pillaging_power.reward_effect(owned_areas), 2);

    pillaging_power.decline_effect(owned_areas);
    BOOST_CHECK_EQUAL(pillaging_power.reward_effect(owned_areas), 0);
}