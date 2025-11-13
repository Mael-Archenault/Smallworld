#include "engine.h"

#include <unordered_map>

#include "state.h"

namespace engine
{

std::unordered_map<int, state::Turn_Phase> phase_command_map = {
    {Conquer_Command::id, state::Turn_Phase::CONQUER},
    {Redeploy_Command::id, state::Turn_Phase::REDEPLOY}
    //{Gather_Free_Units_Command::id, Turn_Phase::GATHER_FREE_UNITS},
    //{Take_Tribe_Command::id, Turn_Phase::TAKE_TRIBE},
    //{Go_In_Decline_Command::id, Turn_Phase::GO_IN_DECLINE}
};

Engine::Engine() : state(4) {};

void Engine::add_command(Command command)
{
    command_queue.push(command);
}

void Engine::update()
{
    // take the first command

    Command command = command_queue.front();
    command_queue.pop();
    // Verify if the command is valid

    if (state.get_current_player().id != command.player_id)
    {
        return;  // or throw
    }

    if (phase_command_map.at(command.id) != state.get_current_turn_phase())
    {
        return;
    }

    // Execute

    command.execute(state);
}

}  // namespace engine