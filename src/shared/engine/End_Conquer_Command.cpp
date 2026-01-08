#include <json/json.h>
#include <stdexcept>

#include "engine.h"
#include "state.h"

namespace engine
{
const int End_Conquer_Command::id = 5;

End_Conquer_Command::End_Conquer_Command(int player_id) : Command(player_id) {}

void End_Conquer_Command::execute(state::Game_State& state)
{
    if (state.get_current_player().get_redeployable_areas().size() == 0)
    {
        throw std::runtime_error("No areas to redeploy from!");
    }
    state.set_current_turn_phase(state::Turn_Phase::REDEPLOY);
    state.gather_free_units(player_id);
};

int End_Conquer_Command::get_id()
{
    return id;
}

void End_Conquer_Command::to_json(Json::Value& root)
{
    root["command_name"] = "End_Conquer_Command";
    root["player_id"]   = player_id;
}

void End_Conquer_Command::from_json(Json::Value& root)
{
    if (root["command_name"] != "End_Conquer_Command")
    {
        throw std::invalid_argument("End_Conquer_Command::from_json: command_name mismatch");
    }
    set_player_id(root["player_id"].asInt());
}

}  // namespace engine