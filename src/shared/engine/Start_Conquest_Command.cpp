#include "engine.h"
#include "state.h"

namespace engine
{
const int Start_Conquest_Command::id = 6;

Start_Conquest_Command::Start_Conquest_Command(int player_id) : Command(player_id) {}

void Start_Conquest_Command::execute(state::Game_State& state)
{
    state.set_current_turn_phase(state::Turn_Phase::CONQUER);
};

int Start_Conquest_Command::get_id()
{
    return id;
}

}  // namespace engine