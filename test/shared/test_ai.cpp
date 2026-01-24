#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "state.h"
#include "ai.h"


int play_ai(std::vector<state::Player_Type> ais_type, std::vector<std::string> ais_names);

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Ai)
{
    std::vector<state::Player_Type> ais = {state::Random_AI,state::Heuristic_AI,state::Advanced_AI};
    int number_of_games = 1;

    std::vector<std::string> ais_names = {};
    int player_id = 0;
    for (state::Player_Type ai_type : ais) {
        switch (ai_type)
        {
            case ai::Ai_Random_t:
                ais_names.push_back("Random_Ai_" + std::to_string(player_id));
                player_id++;
                break;
            case ai::Ai_Heuristic_t:
                ais_names.push_back("Heuristic_Ai_"+ std::to_string(player_id));
                player_id++;
                break;
            case ai::Ai_Advanced_t:
                ais_names.push_back("Advanced_Ai_"+ std::to_string(player_id));
                player_id++;
                break;
            default:
                break;
        }
    }

    std::vector<int> win_count = std::vector<int>(ais.size(),0);

    for (int i = 0; i < number_of_games; i++) {
        win_count.at(play_ai(ais,ais_names)) += 1;
        std::cout << "Finished game " << i << std::endl;
    }

    for (int i = 0; i < win_count.size(); i++) {
        std::cout << "Ai " << ais_names.at(i) << " won " << win_count.at(i)<<" times." << std::endl;
    }
}









int play_ai(std::vector<state::Player_Type> ais_type, std::vector<std::string> ais_names) {
    int winner = -1;
    int player_id = 0;

    std::vector<ai::Ai_Interface*> ais;

    for (state::Player_Type ai_type : ais_type) {
        switch (ai_type)
        {
            case ai::Ai_Random_t:
                ais.emplace_back(new ai::Ai_Random(player_id,ais_names));
                player_id++;
                break;
            case ai::Ai_Heuristic_t:
                ais.emplace_back(new ai::Ai_Heuristic(player_id,ais_names));
                player_id++;
                break;
            case ai::Ai_Advanced_t:
                ais.emplace_back(new ai::Ai_Advanced(player_id, ais_names));
                player_id++;
                break;
            default:
                break;
        }
    }

    engine::Engine   engine = engine::Engine(ais_names);
    state::Game_State& state = engine.get_state();

    while (! state.is_game_finished()) {
        int current_player_id = state.get_current_player().id;
        ais.at(current_player_id)->update_state(state);
        std::shared_ptr<engine::Command> command = ais.at(current_player_id)->give_command(state.get_current_turn_phase());
        engine.add_command(command);
        try {
            engine.update();
        }
        catch (std::exception& e) {
            engine.remove_last_command();
            std::cerr << e.what() << std::endl;
        }
    }
    winner = engine.get_state().get_winner_id();
    return winner;
}