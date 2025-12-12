#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Merchant_Power)
{
    std::vector<state::Area*> owned_areas;
    state::Area               area1(2, 5, state::Area_Biome::FOREST, {}, false);
    state::Area               area2(1, 3, state::Area_Biome::SWAMP, {}, false);
    state::Area               area3(0, 2, state::Area_Biome::HILL, {}, false);

    owned_areas.push_back(&area1);
    owned_areas.push_back(&area2);
    owned_areas.push_back(&area3);
    effects::Merchant_Power merchant_power;

    BOOST_CHECK_EQUAL(merchant_power.rewards_effect(owned_areas), 3);
    merchant_power.decline_effect(owned_areas);
    BOOST_CHECK_EQUAL(merchant_power.rewards_effect(owned_areas), 0);
}