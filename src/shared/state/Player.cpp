#include "Player.h"
#include <stdexcept>
#include <iostream>

namespace state {

int Player::next_id = 0;

Player::Player():id(next_id++) {
    money = 0;
    tribes = std::vector<Tribe*>();
}

void Player::gather_free_units(int tribe_id) {

    for(int i=0; i<tribes.size(); i++){
        if(tribe_id == tribes[i]->id){
            tribes[i]->gather_free_units();
            return;
        }
    }
    throw std::invalid_argument("gather_free_units:there is no tribe_id matching for this player");
}

int Player::get_free_units_number(int tribe_id) {
    for(int i=0; i<tribes.size(); i++){
        if(tribe_id == tribes[i]->id){
            return tribes[i]->get_free_units_number();
        }
    }
    throw std::invalid_argument("get_free_units_number:there is no tribe_id matching for this player");
}

std::vector<std::vector<int>> Player::get_conquest_prices(int tribe_id) {
    for(int i=0; i<tribes.size(); i++){
        if(tribe_id == tribes[i]->id){
            return tribes[i]->get_conquest_prices();
        }
    }
    throw std::invalid_argument("get_conquest_prices:there is no tribe_id matching for this player");
}

void Player::redeploy_units(int tribe_id, int area_id, int n_added_units) {
    for(int i=0; i<tribes.size(); i++){
        if(tribe_id == tribes[i]->id){
            tribes[i]->redeploy_units(area_id, n_added_units);
            return;
        }
    }
    throw std::invalid_argument("redeploy_unit:there is no tribe_id matching for this player");
}

void Player::conquer(int attacking_tribe_id, Area* attacked_area, int n_units, int dice_units){
    for(int i=0; i<tribes.size(); i++){
        if(attacking_tribe_id == tribes[i]->id){
            tribes[i]->conquer(attacked_area, n_units, dice_units);
            return;
        }
    }
    throw std::invalid_argument("conquer:there is no tribe_id matching for this player");
}

void Player::get_rewards(){
    for(int i=0;i<tribes.size();i++){
        money += tribes[i]->get_rewards(); // tribes[i]->get_rewards();
    }
}

void Player::go_in_decline(){
    if (tribes.size()==0){
        return;
    }
    tribes.at(tribes.size()-1)->go_in_decline(); // last tribe is always the active one
}

void Player::set_active_tribe(Tribe* tribe){
    tribes.push_back(tribe);
}

}
