#include <boost/test/unit_test.hpp>
#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Mounted_Power)
{
    std::vector<state::Area*> owned_areas;

    state::Area area1(0, 5, state::Area_Biome::HILL, {}, false);
    state::Area area2(1, 3, state::Area_Biome::FARM, {}, false);
    state::Area area3(2, 4, state::Area_Biome::MOUNTAINS, {}, false);
    state::Area area4(3, 2, state::Area_Biome::FOREST, {}, false);

    state::Tribe tribe(0, new effects::Mounted_Power(),
                       new effects::Power_Description("Test Power", 3));

    area1.set_owner_tribe(&tribe);
    area2.set_owner_tribe(&tribe);

    owned_areas.push_back(&area1);
    owned_areas.push_back(&area2);

    effects::Mounted_Power mounted_power;

    std::vector<state::Area*> initial_areas = { &area1, &area2, &area3, &area4 };

    std::vector<std::pair<int,int>> modified_prices =
        mounted_power.conquest_prices_effect(initial_areas, owned_areas, nullptr);

    // Vérifie la réduction pour HILLS et FARM (min 1)
    for (auto& p : modified_prices)
    {
        if (p.first == area1.id) // HILLS
            BOOST_CHECK_EQUAL(p.second, std::max(1, area1.get_conquest_price(*tribe.get_species()) - 1));
        if (p.first == area2.id) // FARM
            BOOST_CHECK_EQUAL(p.second, std::max(1, area2.get_conquest_price(*tribe.get_species()) - 1));
        if (p.first == area3.id) // MOUNTAINS
            BOOST_CHECK_EQUAL(p.second, area3.get_conquest_price(*tribe.get_species()));
        if (p.first == area4.id) // FOREST
            BOOST_CHECK_EQUAL(p.second, area4.get_conquest_price(*tribe.get_species()));
    }
}
