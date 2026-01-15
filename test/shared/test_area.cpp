#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"

/*
Species_Description::Species_Description(std::string name, int given_units_number, int
max_units_number, Effects_Bundle effects) Power_Description::Power_Description(std::string name, int
given_units_number, Effects_Bundle effects)
*/

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(testarea)
{
    std::vector<state::Area_Specialization> specs = {state::Area_Specialization::MINE,
                                                     state::Area_Specialization::MAGIC_REGION};
    state::Area                             area(0, 6, state::Area_Biome::MOUNTAINS, specs, false);

    // test initialization
    BOOST_CHECK_EQUAL(area.id, 0);
    BOOST_CHECK_EQUAL(static_cast<int>(area.get_biome()),
                      static_cast<int>(state::Area_Biome::MOUNTAINS));

    auto tokens = area.get_area_specialization();
    BOOST_CHECK_EQUAL(tokens.size(), 2);
    BOOST_CHECK_EQUAL(tokens[0], state::Area_Specialization::MINE);
    BOOST_CHECK_EQUAL(tokens[1], state::Area_Specialization::MAGIC_REGION);

    BOOST_CHECK_EQUAL(area.get_units_number(), 6);

    BOOST_CHECK_EQUAL(area.is_border, false);

    BOOST_CHECK_EQUAL(area.get_special_tokens().size(), 1);
    BOOST_CHECK_EQUAL(area.get_special_tokens().at(0), state::Area_Special_Token::MOUNTAIN);
    BOOST_CHECK_EQUAL(area.get_area_specialization().size(), 2);
    BOOST_CHECK_EQUAL(area.get_area_specialization().at(0), state::Area_Specialization::MINE);
    BOOST_CHECK_EQUAL(area.get_area_specialization().at(1),
                      state::Area_Specialization::MAGIC_REGION);

    BOOST_CHECK_EQUAL(area.get_biome(), state::Area_Biome::MOUNTAINS);

    // test gather_free_units
    int free_units = area.gather_free_units();
    BOOST_CHECK_EQUAL(free_units, 5);
    BOOST_CHECK_EQUAL(area.get_units_number(), 1);
    free_units += area.gather_free_units();
    BOOST_CHECK_EQUAL(free_units, 5);

    // test deploy_units
    area.deploy_units(4);
    BOOST_CHECK_EQUAL(area.get_units_number(), 5);

    // test set_units_number
    area.set_units_number(10);
    BOOST_CHECK_EQUAL(area.get_units_number(), 10);

    // test neighbors
    state::Area neighbor(2, 3, state::Area_Biome::HILL, specs, false);
    area.add_neighbor(&neighbor);

    // test add token
    area.add_special_token(state::Area_Special_Token::FORTRESS);
    BOOST_CHECK_EQUAL(area.get_special_tokens().size(), 2);
    BOOST_CHECK_EQUAL(area.get_special_tokens().at(1), state::Area_Special_Token::FORTRESS);

    std::vector<state::Area*> ret_neighbors = area.get_neighbors();
    BOOST_CHECK_EQUAL(ret_neighbors.size(), 1);
    BOOST_CHECK(ret_neighbors.at(0) == &neighbor);

    // test change_owner
    effects::Species_Description species1("Trolls", 5, 10);
    effects::Power_Description   power1("Berserk", 4);

    state::Tribe tribe(0, &species1, &power1);

    area.change_owner(&tribe);
    int price = area.get_conquest_price(tribe);

    BOOST_CHECK(price > 0);
    BOOST_CHECK_EQUAL(area.get_owner_tribe(), &tribe);
}
