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
    std::vector<state::Player> get_players() { return players; }
};

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestGameState)
{
    {
        Game_State_Observer state(1);

        // error_testing
        BOOST_CHECK_THROW(state.gather_free_units(100), std::invalid_argument);
        BOOST_CHECK_THROW(state.get_free_units_number(100), std::invalid_argument);
        BOOST_CHECK_THROW(state.get_conquest_prices(100), std::invalid_argument);
        BOOST_CHECK_THROW(state.conquer(0, 100, 10, 2), std::invalid_argument);
        BOOST_CHECK_THROW(state.conquer(100,0,0,0), std::invalid_argument);
        BOOST_CHECK_THROW(state.redeploy_units(100, 0, 5), std::invalid_argument);
        BOOST_CHECK_THROW(state.get_rewards(100), std::invalid_argument);
        BOOST_CHECK_THROW(state.go_in_decline(100), std::invalid_argument);
        BOOST_CHECK_THROW(state.take_tribe_at_position(0, 100), std::invalid_argument);


        // initialization testing
        BOOST_CHECK_EQUAL(state.get_n_players(), 1);
        BOOST_CHECK_EQUAL(state.get_round(), 0);
        BOOST_CHECK_EQUAL(state.get_players().size(), 1);
        BOOST_CHECK_EQUAL(state.get_map().get_name(), "4_players");
        BOOST_CHECK_EQUAL(state.get_tribe_stack().get_tribes_on_top().size(), 6);
        BOOST_CHECK_EQUAL(state.get_current_player().id, 0);

        // methods testing


        std::vector<state::Tribe*> available_tribes = state.get_tribes_on_top();
        state.take_tribe_at_position(0, 0);
        
        std::vector<std::pair<int, int>> prices = state.get_conquest_prices(0);
        std::vector<std::pair<int, int>> attackable_areas;
        for (const auto& price_info : prices) {
            if (price_info.second <= state.get_free_units_number(0)) {
                attackable_areas.push_back(price_info);
            }
        }

        state.conquer(0,attackable_areas.at(0).first,attackable_areas.at(0).second,0);
        state.gather_free_units(0);
        state.redeploy_units(0,attackable_areas.at(0).first,0);
        state.get_rewards(0);
        state.go_in_decline(0);
        
        state.take_tribe_at_position(0, 0);
        state.abandon_area();
        int dice_units = state.roll_dice_for_bonus_units();

        state.next_round();
        BOOST_CHECK_EQUAL(state.get_round(), 1);
       


    }
}
