#include <json/json.h>

#include <iostream>

#include "engine.h"
#include "state.h"

namespace engine
{
const int Conquer_Command::id = 1;

Conquer_Command::Conquer_Command(int attacking_player_id, int attacked_area_id, int n_units,
                                 bool need_dice_units)
    :

      Command(attacking_player_id),
      attacked_area_id(attacked_area_id),
      n_units(n_units),
      need_dice_units(need_dice_units)
{
}

void Conquer_Command::execute(state::Game_State& state)
{
    int dice_units = -1;
    if (need_dice_units) {
        dice_units = state.roll_dice_for_bonus_units();
    }

    state.conquer(player_id, attacked_area_id, n_units, dice_units);


    if ((dice_units != -1) | (state.get_free_units_number(player_id) == 0))  // dice has been rolled : it's the last conquest
    {
        state.set_current_turn_phase(state::Turn_Phase::REDEPLOY);
        state.gather_free_units(player_id);
    }
};

int Conquer_Command::get_id()
{
    return id;
}

void Conquer_Command::to_json(Json::Value& root)
{
    root["command_name"]     = "Conquer_Command";
    root["attacked_area_id"] = attacked_area_id;
    root["n_units"]          = n_units;
    root["dice_units"]       = dice_units;
    root["attacking_player"] = player_id;
}

void Conquer_Command::from_json(Json::Value& root)
{
    if (root["command_name"] != "Conquer_Command")
    {
        throw std::invalid_argument("Conquer_Command::from_json: command_name mismatch");
    }
    attacked_area_id = root["attacked_area_id"].asInt();
    n_units          = root["n_units"].asInt();
    dice_units       = root["dice_units"].asInt();
    set_player_id(root["attacking_player"].asInt());
}
}  // namespace engine