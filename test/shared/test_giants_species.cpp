#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Humans_Species)
{
    std::vector<state::Area*> owned_areas;
    state::Area               area1(0, 5, state::Area_Biome::MOUNTAINS, {}, false);
    state::Area               area2(1, 3, state::Area_Biome::HILL, {}, false);
    state::Area               area3(2, 4, state::Area_Biome::FARM, {}, false);
    state::Area               area4(3, 2, state::Area_Biome::HILL, {}, false);

    state::Tribe tribe(0, new effects::Giants_Species(),
                       new effects::Power_Description("Test Power", 3));
    area1.set_owner_tribe(&tribe);
    area3.set_owner_tribe(&tribe);

    area1.add_neighbor(&area2);
    area3.add_neighbor(&area4);

    owned_areas.push_back(&area1);
    owned_areas.push_back(&area3);

    effects::Giants_Species giants_species;

    std::vector<std::pair<int, int>> initial_conquest_prices = {{area2.id, 5}, {area3.id, 4}};
    std::vector<std::pair<int, int>> modified_prices =
        giants_species.conquest_prices_effect(initial_conquest_prices, owned_areas);
    BOOST_CHECK_EQUAL(modified_prices.at(0).second, 4);  // price reduced by 1
    BOOST_CHECK_EQUAL(modified_prices.at(1).second, 4);  // initial price
}