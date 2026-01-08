#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Trolls_Species)
{
    std::vector<state::Area*> owned_areas;
    state::Area               area1(0, 5, state::Area_Biome::MOUNTAINS, {}, false);
    state::Area               area2(1, 3, state::Area_Biome::HILL, {}, false);

    owned_areas.push_back(&area1);
    owned_areas.push_back(&area2);

    effects::Trolls_Species trolls_species;

    trolls_species.redeploy_effect(owned_areas);
    BOOST_CHECK_EQUAL(owned_areas.at(0)->get_special_tokens().size(),
                      2);  // moutains and troll lair
    BOOST_CHECK_EQUAL(owned_areas.at(0)->get_special_tokens().at(1),
                      state::Area_Special_Token::TROLL_LAIR);

    BOOST_CHECK_EQUAL(owned_areas.at(1)->get_special_tokens().size(), 1);  // only troll lair
    BOOST_CHECK_EQUAL(owned_areas.at(1)->get_special_tokens().at(0),
                      state::Area_Special_Token::TROLL_LAIR);

    trolls_species.redeploy_effect(owned_areas);  // call on areas that already have troll lairs

    trolls_species.lose_effect(&area1);

    BOOST_CHECK_EQUAL(owned_areas.at(0)->get_special_tokens().size(), 1);  // only mountains left

    trolls_species.disappearing_effect(owned_areas);

    BOOST_CHECK_EQUAL(owned_areas.at(1)->get_special_tokens().size(), 0);  // all tokens removed

    // reaching the maximum troll lairs (10)
    std::vector<state::Area*> more_areas;
    for (int i = 0; i < 15; i++)
    {
        state::Area* new_area = new state::Area(i + 2, 4, state::Area_Biome::HILL, {}, false);
        more_areas.push_back(new_area);
    }

    trolls_species.redeploy_effect(more_areas);
    int troll_lair_count = 0;
    for (state::Area* area : more_areas)
    {
        if (std::find(area->get_special_tokens().begin(), area->get_special_tokens().end(),
                      state::Area_Special_Token::TROLL_LAIR) != area->get_special_tokens().end())
        {
            troll_lair_count++;
        }
    }
    BOOST_CHECK_EQUAL(troll_lair_count, 10);  // only 10 troll lairs
}