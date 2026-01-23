#include <state.h>

#include <boost/test/unit_test.hpp>
#include <fstream>

class Game_State_Observer : public state::Game_State
{
   public:
    Game_State_Observer(std::vector<std::string> names) : state::Game_State(names, 0) {};
    int get_n_players()
    {
        return n_players;
    }
    int get_round()
    {
        return round;
    }
};

BOOST_AUTO_TEST_CASE(TestGameState)
{
    {
        std::vector<std::string> names = {"Alice", "Bob", "Charlie", "Diana"};
        Game_State_Observer      state(names);

        // error_testing
        BOOST_CHECK_THROW(state.gather_free_units(100), std::invalid_argument);
        BOOST_CHECK_THROW(state.get_free_units_number(100), std::invalid_argument);
        BOOST_CHECK_THROW(state.get_conquest_prices(100), std::invalid_argument);
        BOOST_CHECK_THROW(state.conquer(0, 100, 10, 2), std::invalid_argument);
        BOOST_CHECK_THROW(state.conquer(100, 0, 0, 0), std::invalid_argument);
        BOOST_CHECK_THROW(state.redeploy_units(100, 0, 5), std::invalid_argument);
        BOOST_CHECK_THROW(state.get_rewards(100), std::invalid_argument);
        BOOST_CHECK_THROW(state.go_in_decline(100), std::invalid_argument);
        BOOST_CHECK_THROW(state.take_tribe_at_position(0, 100), std::invalid_argument);
        BOOST_CHECK_THROW(state.get_redeployable_areas(100), std::invalid_argument);

        // initialization testing
        BOOST_CHECK_EQUAL(state.get_n_players(), 4);
        BOOST_CHECK_EQUAL(state.get_round(), 1);
        BOOST_CHECK_EQUAL(state.get_players().size(), 4);
        BOOST_CHECK_EQUAL(state.get_map().get_name(), "4_players");
        BOOST_CHECK_EQUAL(state.get_tribe_stack().get_tribes_on_top().size(), 6);
        BOOST_CHECK_EQUAL(state.get_current_player().id, 0);
        BOOST_CHECK_EQUAL(state.get_current_turn_phase(), state::Turn_Phase::START);
        // methods testing

        std::vector<std::pair<std::string, int>> money = state.get_players_money();
        for (int i = 0; i < 4; i++)
        {
            BOOST_CHECK_EQUAL(money.at(i).first, names.at(i));
            BOOST_CHECK_EQUAL(money.at(i).second, 5);
        }
        std::vector<state::Tribe*> available_tribes = state.get_tribes_on_top();
        state.take_tribe_at_position(0, 0);

        state.set_current_turn_phase(state::Turn_Phase::CONQUER);
        BOOST_CHECK_EQUAL(state.get_current_turn_phase(), state::Turn_Phase::CONQUER);

        std::vector<std::pair<int, int>> prices = state.get_conquest_prices(0);
        std::vector<std::pair<int, int>> attackable_areas;
        for (const auto& price_info : prices)
        {
            if (price_info.second <= state.get_free_units_number(0))
            {
                attackable_areas.push_back(price_info);
            }
        }

        state.conquer(0, attackable_areas.at(0).first, attackable_areas.at(0).second, 0);
        BOOST_CHECK_EQUAL(state.get_redeployable_areas(0).size(), 1);
        state.gather_free_units(0);
        state.redeploy_units(0, attackable_areas.at(0).first, 0);
        state.get_rewards(0);
        state.go_in_decline(0);

        state.take_tribe_at_position(0, 0);
        state.abandon_area();
        state.roll_dice_for_bonus_units();

        state.next_player();
        BOOST_CHECK_EQUAL(state.get_current_player().id, 1);
        state.next_round();
        BOOST_CHECK_EQUAL(state.get_round(), 2);

        state::Game_State copy = state.deep_copy();
        Json::Value       root;
        state.to_json(root);
        state.from_json(root);
    }
}
