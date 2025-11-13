#include "engine.h"
#include "state.h"

namespace engine
{
const int Command::id = 6;

End_Start_Command::End_Start_Command(int player_id) : Command(player_id) {}

void End_Start_Command::execute(state::Game_State& state)
{
    state.set_current_turn_phase(state::Turn_Phase::CONQUER);
};

}  // namespace engine