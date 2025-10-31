#include <boost/test/unit_test.hpp>

#include <state.h>
#include <fstream>

class Game_State_Observer : public state::Game_State
{
public:
    Game_State_Observer(int n_players) : state::Game_State(n_players) {
    };
    int get_n_players() { return n_players; }
    int get_round() { return round; }
    state::Map get_map() { return map; }
    std::vector<state::Player> get_players() { return players; }
    state::Tribe_Stack get_tribe_stack() { return tribe_stack; }
};

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestGameState)
{
    {
        Game_State_Observer observer(1);

        // error_testing
        BOOST_CHECK_THROW(observer.gather_free_units(100, 0), std::invalid_argument);
        BOOST_CHECK_THROW(observer.get_free_units_number(100, 0), std::invalid_argument);
        BOOST_CHECK_THROW(observer.get_conquest_prices(100, 0), std::invalid_argument);
        BOOST_CHECK_THROW(observer.conquer(100, 100, 0, 10, 2), std::invalid_argument);
        BOOST_CHECK_THROW(observer.redeploy_units(100, 0, 0, 5), std::invalid_argument);
        BOOST_CHECK_THROW(observer.get_rewards(100), std::invalid_argument);
        BOOST_CHECK_THROW(observer.go_in_decline(100), std::invalid_argument);
        BOOST_CHECK_THROW(observer.take_tribe_at_position(0, 100), std::invalid_argument);


        // initialization testing
        BOOST_CHECK_EQUAL(observer.get_n_players(), 1);
        BOOST_CHECK_EQUAL(observer.get_round(), 0);
        BOOST_CHECK_EQUAL(observer.get_players().size(), 1);

        // methods testing


        std::vector<state::Tribe*> available_tribes = observer.get_tribes_on_top();
        int tribe_id = available_tribes[0]->id;
        observer.take_tribe_at_position(0, 0);
        observer.gather_free_units(0,tribe_id);
        // observer.get_free_units_number(0,tribe_id);
        // std::vector<std::vector<int>> conquest_prices = observer.get_conquest_prices(0,tribe_id);
        // observer.redeploy_units(0,tribe_id,0,0);
        // observer.get_rewards(0);
        // observer.go_in_decline(0);
        
        // observer.abandon_area();
        // int dice_units = observer.roll_dice_for_bonus_units();
        // observer.conquer(0,tribe_id,0,3,0);

        // observer.next_round();
        // BOOST_CHECK_EQUAL(observer.get_round(), 1);
       


    }
}
