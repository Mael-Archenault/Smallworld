#include <boost/test/unit_test.hpp>
#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Underworld_Power)
{
    std::vector<state::Area*> owned_areas;

    state::Area area1(0, 1, state::Area_Biome::HILL, {state::MINE}, false);
    state::Area area2(1, 3, state::Area_Biome::FARM, {state::CAVERN}, false);


    owned_areas.push_back(&area1);
    owned_areas.push_back(&area2);


    effects::Underworld_Power underworld_power;

    std::vector<std::pair<int,int>> initial_prices = {
        {area1.id, 2},
        {area2.id, 4},
    };

    std::vector<std::pair<int,int>> modified_prices =
        underworld_power.conquest_prices_effect(initial_prices, owned_areas, nullptr);

    for (auto& p : modified_prices)
    {
        if (p.first == area1.id)
            BOOST_CHECK_EQUAL(p.second, 2);
        if (p.first == area2.id)
            BOOST_CHECK_EQUAL(p.second, 3);

    }
}
