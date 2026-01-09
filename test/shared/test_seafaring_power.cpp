#include <boost/test/unit_test.hpp>
#include "effects.h"
#include "state.h"
#include <vector>

BOOST_AUTO_TEST_CASE(Test_Seafaring_Power)
{
    std::vector<state::Area*> owned_areas;

    state::Area area0(0, 1, state::Area_Biome::HILL, {}, false);
    owned_areas.push_back(&area0);

    state::Area area1(1, 1, state::Area_Biome::FOREST, {}, false);
    state::Area sea0(2, 0, state::Area_Biome::WATER, {}, false);

    area0.add_neighbor(&area1);
    area0.add_neighbor(&sea0);

    std::vector<std::pair<int,int>> initial_prices = {
        {area1.id, 2} 
    };

    effects::Seafaring_Power seafaring_power;

    std::vector<std::pair<int,int>> modified_prices =
        seafaring_power.conquest_prices_effect(initial_prices, owned_areas, nullptr);

    
    for (auto& p : modified_prices)
    {
        if (p.first == area1.id)
            BOOST_CHECK_EQUAL(p.second, 2); 
        if (p.first == sea0.id)
            BOOST_CHECK_EQUAL(p.second, 1); 
    }

    
    owned_areas.push_back(&sea0);

    
    bool found_sea = false;
    for (auto* a : owned_areas)
    {
        if (a == &sea0)
        {
            found_sea = true;
            break;
        }
    }
    BOOST_CHECK(found_sea);
}
