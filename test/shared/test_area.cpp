#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Area.h"

using namespace state;

/*
Species_Description::Species_Description(std::string name, int given_units_number, int
max_units_number, Effects_Bundle effects) Power_Description::Power_Description(std::string name, int
given_units_number, Effects_Bundle effects)
*/
class MockSpeciesDescription : public Species_Description
{
   public:
    MockSpeciesDescription() : Species_Description("MockSpecies", 5, 10) {}

    int get_initial_units_number()
    {
        return 5;
    }
};

class MockPowerDescription : public Power_Description
{
   public:
    MockPowerDescription() : Power_Description("MockPower", 3) {}

    int get_initial_units_number()
    {
        return 3;
    }
};

class Area_Observer : public Area
{
   public:
    Area_Observer(int id, int n_units, Area_Biome biome, std::vector<Area_Specialization> area_spec,
                  bool is_border)
        : Area(id, n_units, biome, area_spec, is_border)
    {
    }

    void set_neighbors(const std::vector<Area*>& n)
    {
        neighbors = n;
    }
    Tribe* get_owner_tribe() const
    {
        return owner_tribe;
    }
    bool get_is_border()
    {
        return is_border;
    }
};

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(testarea)
{
    std::vector<Area_Specialization> specs = {Area_Specialization::MINE,
                                              Area_Specialization::MAGIC_REGION};
    Area_Observer                    area(0, 6, Area_Biome::MOUNTAINS, specs, false);

    // test initialization
    BOOST_CHECK_EQUAL(area.id, 0);
    BOOST_CHECK_EQUAL(static_cast<int>(area.get_biome()), static_cast<int>(Area_Biome::MOUNTAINS));

    auto tokens = area.get_area_specialization();
    BOOST_CHECK_EQUAL(tokens.size(), 2);
    BOOST_CHECK_EQUAL(tokens[0], Area_Specialization::MINE);
    BOOST_CHECK_EQUAL(tokens[1], Area_Specialization::MAGIC_REGION);

    BOOST_CHECK_EQUAL(area.get_units_number(), 6);

    BOOST_CHECK_EQUAL(area.get_is_border(), false);

    BOOST_CHECK_EQUAL(area.get_special_tokens().size(), 1);
    BOOST_CHECK_EQUAL(area.get_special_tokens().at(0), Area_Special_Token::MOUNTAIN);
    BOOST_CHECK_EQUAL(area.get_area_specialization().size(), 2);
    BOOST_CHECK_EQUAL(area.get_area_specialization().at(0), Area_Specialization::MINE);
    BOOST_CHECK_EQUAL(area.get_area_specialization().at(1), Area_Specialization::MAGIC_REGION);

    BOOST_CHECK_EQUAL(area.get_biome(), Area_Biome::MOUNTAINS);

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
    Area neighbor(2, 3, Area_Biome::HILL, specs, false);
    area.add_neighbor(&neighbor);

    // test add token
    area.add_special_token(Area_Special_Token::FORTRESS);
    BOOST_CHECK_EQUAL(area.get_special_tokens().size(), 2);
    BOOST_CHECK_EQUAL(area.get_special_tokens().at(1), Area_Special_Token::FORTRESS);

    std::vector<Area*> ret_neighbors = area.get_neighbors();
    BOOST_CHECK_EQUAL(ret_neighbors.size(), 1);
    BOOST_CHECK(ret_neighbors.at(0) == &neighbor);

    // test set_owner_tribe
    MockSpeciesDescription species1;
    MockPowerDescription   power1;

    Tribe tribe(0, &species1, &power1);

    area.set_owner_tribe(&tribe);
    int price = area.get_conquest_price(tribe);

    BOOST_CHECK(price > 0);
    BOOST_CHECK_EQUAL(area.get_owner_tribe(), &tribe);
}
