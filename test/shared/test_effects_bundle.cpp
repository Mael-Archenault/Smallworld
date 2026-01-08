#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Amazons_Species)
{
    effects::Effects_Bundle effects_bundle;

    BOOST_CHECK_EQUAL(effects_bundle.first_gather_effect(10), 10);
    std::vector<std::pair<int, int>> prices = {};
    effects_bundle.conquest_prices_effect(prices, {}, nullptr);
    effects_bundle.conquest_effect(nullptr);
    BOOST_CHECK_EQUAL(effects_bundle.second_gather_effect(5), 5);
    effects_bundle.redeploy_effect({});
    BOOST_CHECK_EQUAL(effects_bundle.rewards_effect({}), 0);
    BOOST_CHECK_EQUAL(effects_bundle.lose_effect(nullptr), 0);
    effects_bundle.decline_effect({});
    effects_bundle.disappearing_effect({});
}