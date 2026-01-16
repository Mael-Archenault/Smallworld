#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(Test_Flying_Power)
{
   effects::Flying_Power flying;

   state::Map map("4_players");

   auto prices = flying.conquest_prices_effect({}, {}, &map);

   BOOST_CHECK_EQUAL(flying.conquest_prices_effect({}, {}, &map).size(),
                      36);

   for (const auto& price_info : prices)
   {
     state::Area& area = map.get_area(price_info.first);
     BOOST_CHECK(area.get_biome() != state::Area_Biome::WATER);
   }
}
