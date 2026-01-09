#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

   std::vector<state::Area*> owned_areas;
    state::Area               area1(0, 5, state::Area_Biome::FOREST, {}, false);
    state::Area               area2(1, 3, state::Area_Biome::HILL, {}, false);
    state::Area               area3(2, 4, state::Area_Biome::FARM, {}, false);

    owned_areas.pushback(area1);

    