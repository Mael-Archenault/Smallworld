#include <state.h>

#include <boost/test/unit_test.hpp>
#include <fstream>

class Map_Observer : public state::Map
{
   public:
    Map_Observer(std::string name) : state::Map(name) {};
    int get_max_round()
    {
        return max_round;
    }
    std::vector<std::vector<int>> get_area_connections()
    {
        return area_connections;
    }
};

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestMap)
{
    {
        Map_Observer map("4_players");

        // initialization testing
        BOOST_CHECK_EQUAL(map.get_name(), "4_players");
        BOOST_CHECK_EQUAL(map.get_areas().size(), 39);

        // error testing
        BOOST_CHECK_THROW(map.get_area(100), std::invalid_argument);
        BOOST_CHECK_THROW(map.load_from_json("non_existing_file.json"), std::runtime_error);

        // methods testing
        BOOST_CHECK_EQUAL(map.get_areas().size(), 39);

        BOOST_CHECK_EQUAL(map.get_area_connections().size(), 39);

        BOOST_CHECK_EQUAL(map.get_area_connections()[0].size(), 2);
        BOOST_CHECK_EQUAL(map.get_area_connections()[1].size(), 4);

        BOOST_CHECK_EQUAL(map.get_area(0).id, 0);

        state::Effects_Bundle       species_effect = state::Dwarf_Effects_Bundle();
        state::Species_Description* species_description =
            new state::Species_Description("test_Species", 5, 10, species_effect);
        state::Effects_Bundle     power_effect = state::Dwarf_Effects_Bundle();
        state::Power_Description* power_description =
            new state::Power_Description("test_Power", 4, power_effect);

        state::Tribe tribe(0, species_description, power_description);
        map.get_starting_points_prices(tribe, false);
        BOOST_CHECK_EQUAL(map.get_starting_points_prices(tribe, true).size(), 39);
    }
}

/* vim: set sw=2 sts=2 et : */
