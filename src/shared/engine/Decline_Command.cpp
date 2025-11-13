#include "engine.h"
#include "state.h"

namespace engine
{
const int Command::id = 4;

Decline_Command::Decline_Command(int player_id) : Command(player_id) {}

void Decline_Command::execute(state::Game_State& state)
{
    state.go_in_decline(player_id);
    state.get_rewards(player_id);
    state.next_player();

    // initiating the turn of the next player
    state.gather_free_units(state.get_current_player().id);
    state.set_current_turn_phase(state::Turn_Phase::START);
};

}  // namespace engine