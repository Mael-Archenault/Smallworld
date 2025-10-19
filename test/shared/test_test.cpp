#include <state.h>
#include <iostream>

class Player_Observer : public state::Player
{
public:
  Player_Observer() : state::Player(){};

  int get_money() { return money; }
  std::vector<state::Tribe*> get_tribes() { return tribes; }
};

int main()
  {
    Player_Observer observer;
    // // testing initialization
    // BOOST_CHECK_EQUAL(observer.get_money(), 0);
    // BOOST_CHECK_EQUAL(observer.get_tribes().size(), 0);
    
    
    
    // // errors testing
    // observer.go_in_decline(); // trying to go in decline without any tribe
    // BOOST_CHECK_THROW(observer.get_free_units_number(100), std::invalid_argument);
    
    // BOOST_CHECK_THROW(observer.gather_free_units(100), std::invalid_argument);
    
    // BOOST_CHECK_THROW(observer.get_conquest_prices(100), std::invalid_argument);
    // BOOST_CHECK_THROW(observer.redeploy_units(100, 0,10), std::invalid_argument);
    
    // state::Tribe tribe1 =state::Tribe(new state::Species_Description("TestSpecies", 5, 10, state::Effects_Bundle()),
    //               new state::Power_Description("TestPower", 3, state::Effects_Bundle()));
    
    
    state::Tribe tribe2 =state::Tribe(new state::Species_Description("TestSpecies", 5, 10, state::Effects_Bundle()),
    new state::Power_Description("TestPower", 3, state::Effects_Bundle()));
    

    observer.set_active_tribe(&tribe2);
    std::cout<<observer.get_tribes().size()<<std::endl;
    // observer.get_free_units_number(0);
    // observer.get_conquest_prices(0);
    // observer.go_in_decline();
    //observer.set_active_tribe(tribe2);
    // //observer.gather_free_units(1);

    //BOOST_CHECK_EQUAL(observer.get_tribes().size(), 2);

  }



