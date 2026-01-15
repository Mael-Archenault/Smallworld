#include <iostream>
#include <json/json.h>
#include <stdexcept>

#include "engine.h"
#include "state.h"

namespace engine
{

const int Redeploy_Command::id = 2;

Redeploy_Command::Redeploy_Command(int player_id, int area_id, int added_units)
    :

      Command(player_id),
      area_id(area_id),
      added_units(added_units)
{
}

void Redeploy_Command::execute(state::Game_State& state)
{
    if (state.get_free_units_number(player_id) != 0)
    {
        state.redeploy_units(player_id, area_id, added_units);
    }

    if (state.get_free_units_number(player_id) == 0)
    {
        state.get_rewards(player_id);
        state.next_player();
        state.set_current_turn_phase(state::Turn_Phase::START);
    }
};

int Redeploy_Command::get_id()
{
    return id;
}

void Redeploy_Command::to_json(Json::Value& root)
{
    root["command_name"] = "Redeploy_Command";
    root["area_id"]      = area_id;
    root["added_units"]  = added_units;
    root["player_id"]    = player_id;
}

void Redeploy_Command::from_json(Json::Value& root)
{
    if (root["command_name"] != "Redeploy_Command")
    {
        throw std::invalid_argument("Redeploy_Command::from_json: command_name mismatch");
    }
    area_id     = root["area_id"].asInt();
    added_units = root["added_units"].asInt();
    set_player_id(root["player_id"].asInt());
}

}  // namespace engine