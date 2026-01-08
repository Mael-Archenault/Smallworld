#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Orcs_Species)
{
    state::Area area(0, 5, state::Area_Biome::MOUNTAINS, {}, false);

    effects::Orcs_Species orcs_species;
    orcs_species.conquest_effect(&area);  // area1 is non-empty

    std::vector<state::Area*> owned_areas;  // not used
    BOOST_CHECK_EQUAL(orcs_species.rewards_effect(owned_areas), 1);
    BOOST_CHECK_EQUAL(orcs_species.rewards_effect(owned_areas),
                      0);  // count should have returned to 0
}