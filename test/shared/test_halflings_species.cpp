#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Halflings_Species)
{
    std::vector<state::Area*> owned_areas;
    state::Area               area1(0, 5, state::Area_Biome::FOREST, {}, false);
    state::Area               area2(1, 3, state::Area_Biome::HILL, {}, false);
    state::Area               area3(2, 4, state::Area_Biome::FARM, {}, false);

    owned_areas.push_back(&area1);
    owned_areas.push_back(&area2);
    owned_areas.push_back(&area3);

    effects::Halflings_Species halflings_species;

    state::Map map("4_players");

    BOOST_CHECK_EQUAL(halflings_species.conquest_prices_effect({}, owned_areas, &map).size(),
                      0);  // already have areas -> input prices
    BOOST_CHECK_EQUAL(halflings_species.conquest_prices_effect({}, {}, &map).size(),
                      36);  // don't have any area -> 39 areas - 3 water zones

    halflings_species.conquest_effect(&area1);
    halflings_species.conquest_effect(&area2);
    halflings_species.conquest_effect(&area3);  // should not place a hole
    BOOST_CHECK_EQUAL(area1.get_special_tokens().size(), 1);
    BOOST_CHECK_EQUAL(area2.get_special_tokens().size(), 1);
    BOOST_CHECK_EQUAL(area3.get_special_tokens().size(), 0);

    halflings_species.decline_effect(owned_areas);  // should remove all holes

    BOOST_CHECK_EQUAL(area1.get_special_tokens().size(), 0);
    BOOST_CHECK_EQUAL(area2.get_special_tokens().size(), 0);
    BOOST_CHECK_EQUAL(area3.get_special_tokens().size(), 0);
}