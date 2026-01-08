#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Skeletons_Species)
{
    state::Area area(0, 5, state::Area_Biome::MOUNTAINS, {}, false);

    effects::Skeletons_Species skeletons_species;
    skeletons_species.conquest_effect(&area);  // area is non-empty
    skeletons_species.conquest_effect(&area);  // area is still non-empty

    std::vector<state::Area*> owned_areas;  // not used
    BOOST_CHECK_EQUAL(skeletons_species.second_gather_effect(0), 1);
    BOOST_CHECK_EQUAL(skeletons_species.second_gather_effect(0),
                      0);  // count should have returned to 0

    skeletons_species.conquest_effect(&area);
    skeletons_species.conquest_effect(&area);

    BOOST_CHECK_EQUAL(skeletons_species.second_gather_effect(20), 20);  // maximum number of pawns
}