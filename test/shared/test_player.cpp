#include <state.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestPlayer)
{
    {
        // instanciating useful classes
        state::Tribe* tribe0 =
            new state::Tribe(0, new state::Species_Description("TestSpecies", 5, 10),
                             new state::Power_Description("TestPower", 3));
        state::Tribe* tribe1 =
            new state::Tribe(1, new state::Species_Description("TestSpecies", 5, 10),
                             new state::Power_Description("TestPower", 3));
        static state::Area area(0, 1, state::Area_Biome::HILL,
                                std::vector<state::Area_Specialization>(), false);
        state::Player      player(0);

        // testing initialization
        BOOST_CHECK_EQUAL(player.get_money(), 5);
        BOOST_CHECK_EQUAL(player.id, 0);
        BOOST_CHECK_EQUAL(player.get_tribes().first, nullptr);
        BOOST_CHECK_EQUAL(player.get_tribes().second, nullptr);

        // errors testing
        BOOST_CHECK_THROW(player.go_in_decline(), std::invalid_argument);
        BOOST_CHECK_THROW(player.get_free_units_number(), std::invalid_argument);
        BOOST_CHECK_THROW(player.gather_free_units(), std::invalid_argument);
        BOOST_CHECK_THROW(player.get_conquest_prices(nullptr), std::invalid_argument);
        BOOST_CHECK_THROW(player.redeploy_units(0, 10), std::invalid_argument);
        BOOST_CHECK_THROW(player.conquer(&area, 5, 2), std::invalid_argument);
        BOOST_CHECK_THROW(player.get_redeployable_areas(), std::invalid_argument);
        BOOST_CHECK_THROW(player.set_active_tribe(tribe0, 10), std::invalid_argument);

        // methods testing
        player.set_active_tribe(tribe0, 0);
        BOOST_CHECK_EQUAL(player.get_tribes().first, tribe0);
        BOOST_CHECK_EQUAL(player.get_tribes().second, nullptr);
        player.go_in_decline();

        player.set_active_tribe(tribe1, 0);
        BOOST_CHECK_EQUAL(player.get_tribes().first, tribe1);
        BOOST_CHECK_EQUAL(player.get_tribes().second, tribe0);

        BOOST_CHECK_EQUAL(player.get_free_units_number(), 8);
        player.gather_free_units();
        player.conquer(&area, 3, 0);
        BOOST_CHECK_EQUAL(player.get_conquest_prices(nullptr).size(), 0);
        BOOST_CHECK_EQUAL(player.get_redeployable_areas().size(), 1);
        player.redeploy_units(0, 5);
        player.get_rewards();
        BOOST_CHECK_EQUAL(player.get_money(), 6);
        player.go_in_decline();
    }
}