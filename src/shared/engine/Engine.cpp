#include "engine.h"

#include <stdexcept>
#include <unordered_map>

#include "state.h"

namespace engine
{

std::unordered_map<int, state::Turn_Phase> phase_command_map = {
    {Decline_Command::id, state::Turn_Phase::START},
    {Choose_Species_Command::id, state::Turn_Phase::START},
    {Start_Conquest_Command::id, state::Turn_Phase::START},
    {Conquer_Command::id, state::Turn_Phase::CONQUER},
    {End_Conquer_Command::id, state::Turn_Phase::CONQUER},
    {Redeploy_Command::id, state::Turn_Phase::REDEPLOY}};

Engine::Engine(state::Game_State& state) : state(state) {};

void Engine::add_command(std::unique_ptr<Command> command)
{
    command_queue.push(std::move(command));
}

void Engine::update()
{
    if (command_queue.empty())
    {
        return;
    }
    // take the first command
    std::unique_ptr<Command>& command = command_queue.front();

    // Verify if the command is valid
    if (state.get_current_player().id != command->player_id)
    {
        throw std::runtime_error("It's not the player's turn!");
    }

    if (phase_command_map[command->get_id()] != state.get_current_turn_phase())
    {
        throw std::runtime_error("Command not allowed in the current phase : " +
                                 std::to_string(static_cast<int>(state.get_current_turn_phase())) +
                                 " for command id : " + std::to_string(command->get_id()));
    }

    // Execute

    command->execute(state);

    // removing command from queue
    command_queue.pop();
}

}  // namespace engine