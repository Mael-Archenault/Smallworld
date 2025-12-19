#include <boost/test/unit_test.hpp>
#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Mounted_Power)
{
    std::vector<state::Area*> owned_areas;

    state::Area area1(0, 1, state::Area_Biome::HILL, {}, false);
    state::Area area2(1, 5, state::Area_Biome::FARM, {}, false);
    state::Area area3(2, 3, state::Area_Biome::MOUNTAINS, {}, false);
    state::Area area4(3, 9, state::Area_Biome::FOREST, {}, false);

    owned_areas.push_back(&area1);
    owned_areas.push_back(&area2);
    owned_areas.push_back(&area3);
    owned_areas.push_back(&area4);

    effects::Mounted_Power mounted_power;

    std::vector<state::Area*> initial_areas = { &area1, &area2, &area3, &area4 };

    std::vector<std::pair<int,int>> modified_prices =
        mounted_power.conquest_prices_effect(initial_areas, owned_areas, nullptr);

    for (auto& p : modified_prices)
    {
        if (p.first == area1.id)
            BOOST_CHECK_EQUAL(p.second, 1); 
        if (p.first == area2.id)
            BOOST_CHECK_EQUAL(p.second, 5); 
        if (p.first == area3.id)
            BOOST_CHECK_EQUAL(p.second, 4); 
        if (p.first == area4.id)
            BOOST_CHECK_EQUAL(p.second, 10); 
    }
}
