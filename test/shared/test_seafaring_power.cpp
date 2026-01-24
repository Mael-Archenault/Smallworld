#include <boost/test/unit_test.hpp>
#include <vector>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Seafaring_Power)
{
    std::vector<state::Area*> owned_areas;

    state::Map                       map("4_players");
    std::vector<std::pair<int, int>> initial_prices;

    effects::Seafaring_Power         seafaring_power;
    std::vector<std::pair<int, int>> modified_prices =
        seafaring_power.conquest_prices_effect(initial_prices, owned_areas, &map);

    owned_areas.push_back(&map.get_area(0));

    modified_prices = seafaring_power.conquest_prices_effect(initial_prices, owned_areas, &map);
}
