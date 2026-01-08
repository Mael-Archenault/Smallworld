#include <json/json.h>

#include <stdexcept>

#include "engine.h"
#include "state.h"

namespace engine
{
const int Decline_Command::id = 4;

Decline_Command::Decline_Command(int player_id) : Command(player_id) {}

void Decline_Command::execute(state::Game_State& state)
{
    if (state.get_current_player().get_tribes().first == nullptr)
    {
        throw std::runtime_error("Player has no active tribe!");
    }
    state.go_in_decline(player_id);
    state.get_rewards(player_id);
    state.next_player();
    state.set_current_turn_phase(state::Turn_Phase::START);
};

int Decline_Command::get_id()
{
    return id;
}

void Decline_Command::to_json(Json::Value& root)
{
    root["command_name"] = "Decline_Command";
    root["player_id"]    = player_id;
}

void Decline_Command::from_json(Json::Value& root)
{
    if (root["command_name"] != "Decline_Command")
    {
        throw std::invalid_argument("Decline_Command::from_json: command_name mismatch");
    }
    set_player_id(root["player_id"].asInt());
}

}  // namespace engine