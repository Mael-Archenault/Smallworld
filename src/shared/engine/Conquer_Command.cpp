#include <iostream>

#include "engine.h"
#include "state.h"

namespace engine
{
const int Conquer_Command::id = 1;

Conquer_Command::Conquer_Command(int attacking_player_id, int attacked_area_id, int n_units,
                                 int dice_units)
    :

      Command(attacking_player_id),
      attacked_area_id(attacked_area_id),
      n_units(n_units),
      dice_units(dice_units)
{
}

void Conquer_Command::execute(state::Game_State& state)
{
    try
    {
        state.conquer(player_id, attacked_area_id, n_units, dice_units);
    }

    catch (std::exception& e)
    {
        if (e.what() == std::string("Not enough units to conquer the area!") && dice_units != 0)
        {
            std::cout
                << "Player " << player_id
                << " tried to conquer an area but didn't have enough units even with bonus units."
                << std::endl;
            // removing all bonus units
            state.gather_free_units(player_id);
            state.set_current_turn_phase(state::Turn_Phase::REDEPLOY);
            return;
        }
        else
        {
            throw e;
        }
    }
};

int Conquer_Command::get_id()
{
    return id;
}

}  // namespace engine