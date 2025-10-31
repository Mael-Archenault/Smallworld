#include <boost/test/unit_test.hpp>

#include <state.h>

class Player_Observer : public state::Player
{
public:
  Player_Observer(int id) : state::Player(id){};

  int get_money() { return money; }
  std::vector<state::Tribe*> get_tribes() { return tribes; }
};

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestPlayer)
{
  {
    
    // instanciating useful classes
    state::Tribe* tribe0 = new state::Tribe(
      0,
      new state::Species_Description("TestSpecies", 5, 10, state::Effects_Bundle()),
      new state::Power_Description("TestPower", 3, state::Effects_Bundle())
    );
    state::Tribe* tribe1 = new state::Tribe(
        1,
        new state::Species_Description("TestSpecies", 5, 10, state::Effects_Bundle()),
        new state::Power_Description("TestPower", 3, state::Effects_Bundle())
    );
    static state::Area area(0,1,state::Area_Biome::HILL, std::vector<state::Area_Specialization>());
    Player_Observer observer(0);

    // testing initialization
    BOOST_CHECK_EQUAL(observer.get_money(), 0);
    BOOST_CHECK_EQUAL(observer.get_tribes().size(), 0);
    BOOST_CHECK_EQUAL(observer.id, 0);
    
    // errors testing
    observer.go_in_decline(); // trying to go in decline without any tribe
    BOOST_CHECK_THROW(observer.get_free_units_number(100), std::invalid_argument);
    
    BOOST_CHECK_THROW(observer.gather_free_units(100), std::invalid_argument);
    
    BOOST_CHECK_THROW(observer.get_conquest_prices(100), std::invalid_argument);
    BOOST_CHECK_THROW(observer.redeploy_units(100, 0,10), std::invalid_argument);
    BOOST_CHECK_THROW(observer.conquer(100, &area, 5, 2), std::invalid_argument);


    // methods testing
    observer.set_active_tribe(tribe0); 
    BOOST_CHECK_EQUAL(observer.get_tribes().size(),1);
    observer.go_in_decline();
    
    observer.set_active_tribe(tribe1);
    BOOST_CHECK_EQUAL(observer.get_tribes().size(),2);

    BOOST_CHECK_EQUAL(observer.get_free_units_number(1), 8);
    observer.gather_free_units(1);
    BOOST_CHECK_EQUAL(observer.get_conquest_prices(1).size(), 0);
    observer.redeploy_units(1, 0, 5);
    observer.conquer(1, &area, 3,0);
    observer.get_rewards();
    BOOST_CHECK_EQUAL(observer.get_money(), 0);
  }

}