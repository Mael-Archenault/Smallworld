#include "engine.h"

#include <stdexcept>
#include <unordered_map>

#include "state.h"

bool player_has_an_active_tribe(state::Game_State& state, int player_id)
{
    state::Player& player = state.get_current_player();
    return player.get_tribes().first != nullptr;
}

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

    if (!player_has_an_active_tribe(state, command->player_id) &&
        command->get_id() != Choose_Species_Command::id)  // all commands require an active tribe
                                                          // (except Choose_Species)
    {
        command_queue.pop();
        throw std::runtime_error("Player has no active tribe!");
    }
    if (player_has_an_active_tribe(state, command->player_id) &&
        command->get_id() == Choose_Species_Command::id)  // Choose_Species requires no active tribe
    {
        command_queue.pop();
        throw std::runtime_error("Player already has an active tribe!");
    }
    if (state.get_current_player().id != command->player_id)
    {
        command_queue.pop();
        throw std::runtime_error("It's not the player's turn!");
    }

    if (phase_command_map[command->get_id()] != state.get_current_turn_phase())
    {
        command_queue.pop();
        throw std::runtime_error("Command not allowed in the current phase");
    }

    // Execute

    command->execute(state);

    // removing command from queue
    command_queue.pop();
}

void Engine::remove_last_command()
{
    command_queue.pop();
}

}  // namespace engine