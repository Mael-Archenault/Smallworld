#include <json/json.h>
#include <stdexcept>

#include "engine.h"
#include "state.h"

namespace engine
{
const int Start_Conquest_Command::id = 6;

Start_Conquest_Command::Start_Conquest_Command(int player_id) : Command(player_id) {}

void Start_Conquest_Command::execute(state::Game_State& state)
{
    if (state.get_current_player().get_tribes().first == nullptr)
    {
        throw std::runtime_error("Player has no active tribe!");
    }
    state.set_current_turn_phase(state::Turn_Phase::CONQUER);
    if (state.get_current_player().get_tribes().first != nullptr)
    {
        state.gather_free_units(state.get_current_player().id);
    }
};

int Start_Conquest_Command::get_id()
{
    return id;
}

void Start_Conquest_Command::to_json(Json::Value& root)
{
    root["command_name"] = "Start_Conquest_Command";
    root["player_id"]   = player_id;
}

void Start_Conquest_Command::from_json(Json::Value& root)
{
    if (root["command_name"] != "Start_Conquest_Command")
    {
        throw std::invalid_argument("Start_Conquest_Command::from_json: command_name mismatch");
    }
    set_player_id(root["player_id"].asInt());
}

}  // namespace engine