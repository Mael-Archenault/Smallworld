#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Commando_Power)
{
    effects::Alchemist_Power  alchemist_power;
    std::vector<state::Area*> owned_areas;
    BOOST_CHECK_EQUAL(alchemist_power.rewards_effect(owned_areas), 2);
    alchemist_power.decline_effect(owned_areas);
    BOOST_CHECK_EQUAL(alchemist_power.rewards_effect(owned_areas), 0);
}
