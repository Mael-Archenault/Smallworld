#include <state.h>

#include <boost/test/unit_test.hpp>

class Power_Description_Observer : public state::Power_Description
{
   public:
    Power_Description_Observer(std::string name, int given_units_number)
        : state::Power_Description(name, given_units_number) {};

    int get_given_units_number()
    {
        return given_units_number;
    }
    std::string get_name()
    {
        return name;
    }
};

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestPowerDescription)
{
    {
        // testing initialization
        Power_Description_Observer power_description("TestPower", 5);
        BOOST_CHECK_EQUAL(power_description.get_name(), "TestPower");
        BOOST_CHECK_EQUAL(power_description.get_given_units_number(), 5);

        // testing methods

        BOOST_CHECK_EQUAL(power_description.get_initial_units_number(), 5);
        std::vector<state::Area_Specialization> area_specs;
        state::Area               area(0, 10, state::Area_Biome::HILL, area_specs, false);
        std::vector<state::Area*> areas;
        areas.push_back(&area);
        // bonus reward not implemented yet so it should return 0
        BOOST_CHECK_EQUAL(power_description.get_bonus_rewards(areas), 0);

        BOOST_CHECK_EQUAL(power_description.get_name(), "TestPower");
    }

    {
    }
}