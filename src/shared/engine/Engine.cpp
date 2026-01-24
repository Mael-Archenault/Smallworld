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

Engine::Engine(std::vector<std::string> names) : state(names, 1) {};

void Engine::add_command(std::shared_ptr<Command> command)
{
    command_queue.push(command);
}

void Engine::update()
{
    if (command_queue.empty())
    {
        return;
    }
    // take the first command
    std::shared_ptr<Command>& command = command_queue.front();

    // Verify if turn's conditions

    if (state.get_current_player().id != command->player_id)
    {
        throw std::runtime_error("It's not the player's turn!, player "+std::to_string(state.get_current_player().id)+" tried.");
    }

    if (phase_command_map[command->get_id()] != state.get_current_turn_phase())
    {
        throw std::runtime_error("Command not allowed in the current phase");
    }

    // Execute

    command->execute(state);

    // removing command from queue
    command_queue.pop();
    state.new_version_id();
}

void Engine::remove_last_command()
{
    command_queue.pop();
}

state::Game_State& Engine::get_state()
{
    return state;
}
int Engine::get_state_version_id()
{
    return state.get_version_id();
}

void Engine::add_command(Json::Value command_json)
{
    std::shared_ptr<Command> command = Command::create_from_json(command_json);
    command_queue.push(std::move(command));
}

Json::Value Engine::get_state_json()
{
    Json::Value root;
    state.to_json(root);
    return root;
}
}  // namespace engine