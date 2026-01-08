#include <boost/test/unit_test.hpp>
#include <iostream>

#include "effects.h"
#include "state.h"

class Tribe_Observer : public state::Tribe
{
   public:
    Tribe_Observer(int id, effects::Species_Description* species_description,
                   effects::Power_Description* power_description)
        : Tribe(id, species_description, power_description) {};
    std::vector<state::Area*> get_owned_areas()
    {
        return owned_areas;
    }
};

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Tribe_Test)
{
    // Initialization
    effects::Species_Description* species_description =
        new effects::Species_Description("test_Species", 5, 10);
    effects::Power_Description* power_description = new effects::Power_Description("test_Power", 4);

    Tribe_Observer tribe(0, species_description, power_description);

    std::vector<state::Area*> areas;
    areas.emplace_back(new state::Area(0, 3, state::Area_Biome::HILL, {}, false));
    areas.emplace_back(new state::Area(1, 2, state::Area_Biome::FOREST, {}, false));
    areas.at(0)->add_neighbor(areas.at(1));
    areas.at(1)->add_neighbor(areas.at(0));

    // Initialization testing
    BOOST_CHECK_EQUAL(tribe.get_species_description(), species_description);
    BOOST_CHECK_EQUAL(tribe.get_power_description(), power_description);
    BOOST_CHECK_EQUAL(tribe.get_free_units_number(), 9);
    BOOST_CHECK_EQUAL(tribe.is_in_decline(), false);
    BOOST_CHECK_EQUAL(tribe.get_owned_areas().size(), 0);
    BOOST_CHECK_EQUAL(tribe.get_owner(), nullptr);

    BOOST_CHECK_THROW(tribe.remove_from_owned_areas(areas[0]), std::invalid_argument);

    tribe.conquer(areas.at(0), 5, 0, nullptr);

    BOOST_CHECK_EQUAL(tribe.get_owned_areas().size(), 1);
    BOOST_CHECK_EQUAL(tribe.get_owned_areas().at(0)->id, 0);
    BOOST_CHECK_EQUAL(tribe.get_free_units_number(), 4);
    BOOST_CHECK_EQUAL(tribe.get_redeployable_areas().size(), 1);

    BOOST_CHECK_THROW(tribe.conquer(areas.at(1), 1, -1, nullptr), std::invalid_argument);
    BOOST_CHECK_THROW(tribe.conquer(areas.at(1), 10, -1, nullptr), std::invalid_argument);

    std::vector<std::pair<int, int>> price_infos = tribe.get_conquest_prices(nullptr);

    BOOST_CHECK_EQUAL(price_infos.size(), 1);
    BOOST_CHECK_EQUAL(price_infos.at(0).first, 1);
    BOOST_CHECK_EQUAL(price_infos.at(0).second, 4);

    tribe.gather_free_units(state::Turn_Phase::START);
    BOOST_CHECK_EQUAL(tribe.get_free_units_number(), 8);

    tribe.redeploy_units(0, 7);
    BOOST_CHECK_THROW(tribe.redeploy_units(0, 2), std::invalid_argument);
    BOOST_CHECK_THROW(tribe.redeploy_units(100, 0), std::invalid_argument);
    BOOST_CHECK_EQUAL(tribe.get_free_units_number(), 1);

    BOOST_CHECK_EQUAL(tribe.get_rewards(), 1);  // 1 area -> 1 piece of money

    BOOST_CHECK_EQUAL(tribe.get_species_name(), "test_Species");
    BOOST_CHECK_EQUAL(tribe.get_power_name(), "test_Power");

    tribe.go_in_decline();
    BOOST_CHECK_EQUAL(tribe.is_in_decline(), true);
    tribe.remove_from_map();
    BOOST_CHECK_EQUAL(tribe.get_owned_areas().size(), 0);
}
