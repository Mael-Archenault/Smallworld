//
// Created by julien on 1/8/26.
//


#include "client/Client_AI.h"

#include <mutex>
#include <unistd.h>
#include <engine/Engine.h>

#include <utility>
extern std::mutex mtx;
using namespace client;
Client_AI::Client_AI(engine::Engine& engine, int player_id, ai::Ai_Interface * ai) :
                    state(engine.get_state().deep_copy()),
                    engine(engine),
                    state_was_updated(true),
                    player_id(player_id),
                    ai(ai)

{
}


int Client_AI::run() {
    while (1) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (state.is_game_finished() == true ) {
                return player_id;
            }
        }
        if (state_was_updated) {
            std::lock_guard<std::mutex> lock(mtx);
            state_was_updated = false;
            if (state.get_current_player().id == player_id) {
                ai->update_state(state);
                engine.add_command(ai->give_command(state.get_current_turn_phase()));
            }
        }
        usleep(100000);
    }
}


state::Game_State& Client_AI::get_state()
{
    return state;
}

engine::Engine& Client_AI::get_engine()
{
    return engine;
}

void Client_AI::update_state()
{
    state = engine.get_state().deep_copy();
    state_was_updated = true;
}

int Client_AI::get_player_id()
{
    return player_id;
}
