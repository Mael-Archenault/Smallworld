#include "Game_State.h"

namespace state {

Game_State::Game_State() {
}

void Game_State::gather_free_units(int player_id, int tribe_id) {
}

int Game_State::get_free_units_number(int player_id, int tribe_id) {
}

std::vector<int> Game_State::get_conquest_prices(int player_id, int tribe_id) {
}

void Game_State::conquer(int attacking_player_id, int attacking_tribe_id, int attacked_area_id, int n_units) {
}

void Game_State::roll_dice_for_bonus_units(int player_id, int tribe_id) {
}

void Game_State::deploy_units(int player_id, int tribe_id, int area_id, int n_added_units) {
}

}