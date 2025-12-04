
#include <boost/test/unit_test.hpp>

#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Dwarves_Species)
{
    std::vector<state::Area*> owned_areas;
    state::Area area1(0, 5, state::Area_Biome::MOUNTAINS, {state::Area_Specialization::MINE},
                      false);
    state::Area area2(1, 3, state::Area_Biome::HILL, {}, false);

    owned_areas.push_back(&area1);
    owned_areas.push_back(&area2);

    state::Dwarves_Species dwarves_species;

    BOOST_CHECK_EQUAL(dwarves_species.get_bonus_rewards(owned_areas), 1);
}
