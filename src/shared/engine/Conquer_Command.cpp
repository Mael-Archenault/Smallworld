#include "engine.h"
#include "state.h"

namespace engine
{
const int Command::id = 1;

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

    // stopping the conquests if the player used the dice
    if (dice_units != 0)
    {
        state.set_current_turn_phase(state::Turn_Phase::REDEPLOY);
    }
};

}  // namespace engine