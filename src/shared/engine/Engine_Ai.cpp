//
// Created by julien on 12/5/25.
//

#include "Engine_Ai.h"
namespace engine
{
Engine_Ai::Engine_Ai(std::vector<std::string> names) : Engine(names)
{
    this->state = state.deep_copy();
}

void Engine_Ai::set_state(state::Game_State& state)
{
    this->state = state.deep_copy();
}
}  // namespace engine