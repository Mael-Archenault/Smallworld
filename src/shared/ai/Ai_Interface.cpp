//
// Created by julien on 11/20/25.
//

#include <memory>
#include <random>
#include <stdexcept>

#include "ai.h"
#include "engine.h"

namespace ai
{

Ai_Interface::Ai_Interface(state::Game_State state, int player_id) : id(player_id), state(state) {}

std::shared_ptr<engine::Command> Ai_Interface::give_command(state::Turn_Phase phase)
{
    switch (phase)
    {
        case state::Turn_Phase::START:
            return give_command_Start();
        case state::Turn_Phase::CONQUER:
            return give_command_Conquer();
        case state::Turn_Phase::REDEPLOY:
            return give_command_Redeploy();
        default:
            throw std::runtime_error("Unknown turn phase");
    }
}

std::shared_ptr<engine::Command> Ai_Interface::give_command_Start()
{
    return nullptr;
}

std::shared_ptr<engine::Command> Ai_Interface::give_command_Conquer()
{
    return nullptr;
}

std::shared_ptr<engine::Command> Ai_Interface::give_command_Redeploy()
{
    return nullptr;
}

void Ai_Interface::update_state(state::Game_State new_state)
{
    this->state = new_state.deep_copy();
}

}