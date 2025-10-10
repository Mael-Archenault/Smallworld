#include "Player.h"

namespace state {

Player::Player(const int id):id(id) {
    int money = 0;
    std::vector<Tribe*> tribes;
}

void Player::gather_free_units(int tribe_id) {
    int curr_tribe = Player::tribes.size();
    for(int i=0; i<curr_tribe; i++){
        if(tribe_id == tribes[i]->id){
            tribes[i]->gather_free_units();
            return;
        }
    }
}

int Player::get_free_units_number(int tribe_id) {
    int curr_tribe = Player::tribes.size();
    for(int i=0; i<curr_tribe; i++){
        if(tribe_id == tribes[i]->id){
            return tribes[i]->get_free_units_number();
        }
    }
}

std::vector<std::vector<int>> Player::get_conquest_prices(int tribe_id) {
    int curr_tribe = Player::tribes.size();
    for(int i=0; i<curr_tribe; i++){
        if(tribe_id == tribes[i]->id){
            return tribes[i]->get_conquest_prices();
        }
    }
}

void Player::redeploy_units(int tribe_id, int area_id, int n_added_units) {
    int curr_tribe = Player::tribes.size();
    for(int i=0; i<curr_tribe; i++){
        if(tribe_id == tribes[i]->id){
            tribes[i]->redeploy_units(area_id, n_added_units);
            return;
        }
    }
}

void Player::conquer(int attacking_tribe_id, Area& attacked_area, int n_units, int dice_units){
    int curr_tribe = Player::tribes.size();
    for(int i=0; i<curr_tribe; i++){
        if(attacking_tribe_id == tribes[i]->id){
            tribes[i]->conquer(attacked_area, n_units, dice_units);
            return;
        }
    }
}
}