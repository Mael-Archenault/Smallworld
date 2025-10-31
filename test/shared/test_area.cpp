#include <boost/test/unit_test.hpp>
#include "../../src/shared/state/Area.h"

using namespace state;


/*
Species_Description::Species_Description(std::string name, int given_units_number, int max_units_number, Effects_Bundle effects)
Power_Description::Power_Description(std::string name, int given_units_number, Effects_Bundle effects)
*/
class MockSpeciesDescription : public Species_Description {
public:
    MockSpeciesDescription()
        : Species_Description("MockSpecies", 5, 10, Effects_Bundle{}) {}

    int get_initial_units_number()  {
        return 5;
    }
};

class MockPowerDescription : public Power_Description {
public:
    MockPowerDescription()
        : Power_Description("MockPower", 3, Effects_Bundle{}) {}

    int get_initial_units_number()  {
        return 3;
    }
};

class Area_Observer : public Area {
public:
    Area_Observer(int id, int n_units, Area_Biome biome, std::vector<Area_Specialization> area_spec)
        : Area(id, n_units, biome, area_spec) {}

    int& get_units_number() { return units_number; }
    std::vector<Area_Special_Token>& get_special_tokens() { return special_tokens; }
    Area_Biome& get_biome() { return biome; }
    std::vector<Area_Specialization>& get_area_specialization() { return area_specialization; }
    void set_neighbors(const std::vector<Area*>& n) { neighbors = n; }
    Tribe* get_owner_tribe() const {return owner_tribe;}

};

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(testarea)
{   

    std::vector<Area_Specialization> specs = {Area_Specialization::MINE,Area_Specialization::MAGIC_REGION};
    Area_Observer area(0, 6, Area_Biome::MOUNTAINS, specs);

    //test biome
    BOOST_CHECK_EQUAL(static_cast<int>(area.get_biome()), static_cast<int>(Area_Biome::MOUNTAINS));

    //test specialization
    auto tokens = area.get_area_specialization();
    BOOST_CHECK_EQUAL(tokens.size(), 2);
    BOOST_CHECK_EQUAL(tokens[0],Area_Specialization::MINE);
    BOOST_CHECK_EQUAL(tokens[1],Area_Specialization::MAGIC_REGION);

    //test getUnits_number
    BOOST_CHECK_EQUAL(area.get_units_number(), 6);

    //test gather_free_units
    int free_units = area.gather_free_units();
    BOOST_CHECK_EQUAL(free_units, 5);
    BOOST_CHECK_EQUAL(area.get_units_number(), 1);

    Area_Observer area_low(1,1, Area_Biome::FARM, specs);
    int free_units_low = area_low.gather_free_units();
    BOOST_CHECK_EQUAL(free_units_low, 0);
    BOOST_CHECK_EQUAL(area_low.get_units_number(), 1);

    //test deploy_units
    area.deploy_units(4);
    BOOST_CHECK_EQUAL(area.get_units_number(), 5);

   
    //test set_units_number
    area.set_units_number(10);
    BOOST_CHECK_EQUAL(area.get_units_number(), 10);

    
    //test get_neighbors
    Area_Observer area1(2,3, Area_Biome::HILL, specs);
    Area_Observer area2(3,2, Area_Biome::MOUNTAINS, specs);
    Area_Observer area3(4,5, Area_Biome::FARM, specs);

    std::vector<Area*> neighbors = {&area2, &area3};
    area1.set_neighbors(neighbors);

    std::vector<Area*> ret_neighbors = area1.get_neighbors();
    BOOST_CHECK_EQUAL(ret_neighbors.size(), 2);
    BOOST_CHECK(ret_neighbors[0] == &area2);
    BOOST_CHECK(ret_neighbors[1] == &area3);


    //test set_owner_tribe
    MockSpeciesDescription species1;
    MockPowerDescription power1;

    Tribe tribe(0,&species1, &power1);

    area.set_owner_tribe(&tribe);
    int price = area.get_conquest_price(tribe);

    BOOST_CHECK(price > 0);
    BOOST_CHECK_EQUAL(area.get_owner_tribe(), &tribe);
}
