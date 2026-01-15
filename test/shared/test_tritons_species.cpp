#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Tritons_Species)
{
    std::vector<state::Area*> owned_areas;
    state::Map                map("4_players");
    state::Area               area0 = map.get_area(25);
    state::Area               area1 = map.get_area(27);  // coastal
    state::Area               area2 = map.get_area(11);
    state::Area               area3 = map.get_area(13);  // non coastal

    state::Tribe tribe(0, new effects::Tritons_Species(),
                       new effects::Power_Description("Test Power", 3));

    area0.change_owner(&tribe);
    area2.change_owner(&tribe);

    owned_areas.push_back(&area1);
    owned_areas.push_back(&area3);

    effects::Tritons_Species tritons_species;

    std::vector<std::pair<int, int>> initial_conquest_prices = {{area1.id, 2}, {area3.id, 2}};
    std::vector<std::pair<int, int>> modified_prices =
        tritons_species.conquest_prices_effect(initial_conquest_prices, owned_areas, &map);
    BOOST_CHECK_EQUAL(modified_prices.at(0).second, 1);  // price reduced by 1
    BOOST_CHECK_EQUAL(modified_prices.at(1).second, 2);  // initial price
}