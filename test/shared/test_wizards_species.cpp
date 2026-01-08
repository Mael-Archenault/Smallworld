#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Wizards_Species)
{
    std::vector<state::Area*> owned_areas;
    state::Area               area1(0, 5, state::Area_Biome::MOUNTAINS,
                                    {state::Area_Specialization::MAGIC_REGION}, false);
    state::Area               area2(1, 3, state::Area_Biome::HILL, {}, false);

    owned_areas.push_back(&area1);
    owned_areas.push_back(&area2);

    effects::Wizards_Species wizards_species;

    BOOST_CHECK_EQUAL(wizards_species.rewards_effect(owned_areas), 1);
    wizards_species.decline_effect(owned_areas);
    BOOST_CHECK_EQUAL(wizards_species.rewards_effect(owned_areas), 0);
}