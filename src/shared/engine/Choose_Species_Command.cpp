#include "engine.h"
#include "state.h"

namespace engine
{
const int Command::id = 3;

Choose_Species_Command::Choose_Species_Command(int player_id, int position)
    :

      Command(player_id),
      position(position)
{
}

void Choose_Species_Command::execute(state::Game_State& state)
{
    state.take_tribe_at_position(position, player_id);
    state.set_current_turn_phase(state::Turn_Phase::CONQUER);
};

}  // namespace engine