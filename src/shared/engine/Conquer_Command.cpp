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
    state.conquer(player_id, attacked_area_id, n_units, dice_units);

    if (dice_units != -1 | state.get_free_units_number(player_id) ==
                               0)  // dice has been rolled : it's the last conquest
    {
        state.gather_free_units(player_id);
        state.set_current_turn_phase(state::Turn_Phase::REDEPLOY);
        return;
    }
};

int Conquer_Command::get_id()
{
    return id;
}

}  // namespace engine