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
    state.redeploy_units(player_id, area_id, added_units);
    std::cout << state.get_free_units_number(player_id) << " free units remaining for player "
              << player_id << std::endl;
    if (state.get_free_units_number(player_id) == 0)
    {
        state.get_rewards(player_id);
        state.next_player();
        state.set_current_turn_phase(state::Turn_Phase::START);
        if (state.get_current_player().get_tribes().first != nullptr)
        {
            state.gather_free_units(state.get_current_player().id);
        }
    }
};

int Redeploy_Command::get_id()
{
    return id;
}

}  // namespace engine