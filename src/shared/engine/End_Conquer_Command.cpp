#include "engine.h"
#include "state.h"

namespace engine
{
const int End_Conquer_Command::id = 5;

End_Conquer_Command::End_Conquer_Command(int player_id) : Command(player_id) {}

void End_Conquer_Command::execute(state::Game_State& state)
{
    state.gather_free_units(player_id);
    state.set_current_turn_phase(state::Turn_Phase::REDEPLOY);
};

int End_Conquer_Command::get_id()
{
    return id;
}

}  // namespace engine