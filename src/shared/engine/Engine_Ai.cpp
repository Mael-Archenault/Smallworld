//
// Created by julien on 12/5/25.
//

#include "Engine_Ai.h"
namespace engine {
    Engine_Ai::Engine_Ai(state::Game_State& state, int nb_players) : Engine(nb_players,state.names){
        this->state = state::Game_State(state);
    }

    void Engine_Ai::set_state(state::Game_State& state) {
        this->state = state.deep_copy();
    }
}