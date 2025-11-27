#include <stdexcept>

#include "engine.h"
#include "state.h"

namespace engine
{
const int Start_Conquest_Command::id = 6;

Start_Conquest_Command::Start_Conquest_Command(int player_id) : Command(player_id) {}

void Start_Conquest_Command::execute(state::Game_State& state)
{
    if (state.get_current_player().get_tribes().first == nullptr)
    {
        throw std::runtime_error("Player has no active tribe!");
    }
    state.set_current_turn_phase(state::Turn_Phase::CONQUER);
};

int Start_Conquest_Command::get_id()
{
    return id;
}

}  // namespace engine