#include "engine.h"

#include <unordered_map>

#include "state.h"

namespace engine
{

std::unordered_map<int, state::Turn_Phase> phase_command_map = {
    {Decline_Command::id, state::Turn_Phase::START},
    {Choose_Species_Command::id, state::Turn_Phase::START},
    {End_Start_Command::id, state::Turn_Phase::START},
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
    command_queue.pop();

    // Verify if the command is valid

    // if (state.get_current_player().id != command->player_id)
    // {
    //     return;  // or throw
    // }

    if (phase_command_map.at(command->id) != state.get_current_turn_phase())
    {
        return;
    }

    // // Execute

    // command->execute(state);
}

}  // namespace engine