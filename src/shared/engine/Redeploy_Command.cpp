#include <iostream>
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

}  // namespace engine