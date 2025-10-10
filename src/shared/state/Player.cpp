#include "Player.h"

namespace state {

Player::Player(const int id):id(id){
};

void Player::gather_free_units(int tribe_id) {
}

int Player::get_free_units_number(int tribe_id) {
}

std::vector<int> Player::get_conquest_prices(int tribe_id) {
}

void Player::roll_dice_for_bonus_units(int tribe_id) {
}

void Player::deploy_units(int tribe_id, int area_id, int n_added_units) {
}

void Player::conquer(int attacking_tribe_id, Area& attacked_area, int n_units) {
}

}