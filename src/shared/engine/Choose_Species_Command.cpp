#include <stdexcept>

#include "engine.h"
#include "state.h"

namespace engine
{
const int Choose_Species_Command::id = 3;

Choose_Species_Command::Choose_Species_Command(int player_id, int position)
    : Command(player_id), position(position)
{
}

void Choose_Species_Command::execute(state::Game_State& state)
{
    if (state.get_current_player().get_tribes().first != nullptr)
    {
        throw std::runtime_error("Player already has an active tribe!");
    }
    state.take_tribe_at_position(position, player_id);
    state.set_current_turn_phase(state::Turn_Phase::CONQUER);
};

int Choose_Species_Command::get_id()
{
    return id;
}

}  // namespace engine