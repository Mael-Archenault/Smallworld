#include "engine.h"
#include "state.h"

namespace engine
{
const int Decline_Command::id = 4;

Decline_Command::Decline_Command(int player_id) : Command(player_id) {}

void Decline_Command::execute(state::Game_State& state)
{
    state.go_in_decline(player_id);
    state.get_rewards(player_id);
    state.next_player();
    state.set_current_turn_phase(state::Turn_Phase::START);
    state.gather_free_units(state.get_current_player().id);
};

int Decline_Command::get_id()
{
    return id;
}

}  // namespace engine