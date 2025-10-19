#include <boost/test/unit_test.hpp>

#include <state.h>


class Power_Description_Observer:public state::Power_Description{
    public:
    Power_Description_Observer(std::string name,
                                int given_units_number,
                                state::Effects_Bundle effects): state::Power_Description(name,
                                                                given_units_number,
                                                                effects){};

    int get_given_units_number(){return given_units_number;}
    std::string get_name(){return name;}
};

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestPowerDescription)
{
  {
    // testing initialization
    state::Dwarf_Effects_Bundle bundle = state::Dwarf_Effects_Bundle();
    Power_Description_Observer observer("TestPower", 5, (state::Effects_Bundle)bundle);
    BOOST_CHECK_EQUAL(observer.get_name(), "TestPower");
    BOOST_CHECK_EQUAL(observer.get_given_units_number(), 5);

    // testing methods

    BOOST_CHECK_EQUAL(observer.get_initial_units_number(), 5);
    std::vector<state::Area_Specialization> area_specs;
    state::Area area(10, state::Area_Biome::HILL, area_specs);
    // bonus reward not implemented yet so it should return 0
    BOOST_CHECK_EQUAL(observer.get_bonus_rewards(area), 0);


  }

  {
  }
}