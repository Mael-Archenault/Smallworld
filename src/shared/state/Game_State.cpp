#include "Game_State.h"
#include <stdexcept>

namespace state {

Game_State::Game_State(int n_players) : n_players(n_players), round(0), map(Map("4_players")){
    for (int i=0; i<n_players; i++ ) {
        players.push_back(Player(i));
    }
    current_player = &players[0];
    tribe_stack = Tribe_Stack();
}

void Game_State::gather_free_units(int player_id, int tribe_id) {
    for (int i=0; i<players.size(); i++) {
        if (players[i].id == player_id) {
            players[i].gather_free_units(tribe_id);
            return;
        }
    } 
    throw std::invalid_argument("gather_free_units: Player id not found");
}

int Game_State::get_free_units_number(int player_id, int tribe_id) {
    int result;
    for (int i=0; i<players.size(); i++) {
        if (players[i].id == player_id) {
            return players[i].get_free_units_number(tribe_id);
        }
    }
    throw std::invalid_argument("get_free_units_number: Player id not found");
}

std::vector<std::vector<int>> Game_State::get_conquest_prices(int player_id, int tribe_id) {
    std::vector<std::vector<int>> result;
    for (int i=0; i<players.size(); i++) {
        if (players[i].id == player_id) {
            return players[i].get_conquest_prices(tribe_id);
        }
    }
    throw std::invalid_argument("get_conquest_prices: Player id not found");
}

void Game_State::conquer(int attacking_player_id, int attacking_tribe_id, int attacked_area_id, int n_units, int dice_units) {
    Area* attacked_area = map.get_area(attacked_area_id);
    for (int i=0; i<players.size(); i++) {
        if (players[i].id == attacking_player_id) {
            players[i].conquer(attacking_tribe_id, attacked_area, n_units, dice_units);
            return;
        }
    }
    throw std::invalid_argument("conquer: Player id not found");
}

int Game_State::roll_dice_for_bonus_units() {
    std::vector<int> possible_values = {0,0,0,1,2,3};

    int random_index = std::rand()%5;

    return possible_values[random_index];
}

void Game_State::redeploy_units(int player_id, int tribe_id, int area_id, int n_added_units) {
    for (int i=0; i<players.size(); i++) {
        if (players[i].id == player_id) {
            players[i].redeploy_units(tribe_id, area_id, n_added_units);
            return;
        }
    }
    throw std::invalid_argument("redeploy_units: Player id not found");
}

void Game_State::get_rewards(int player_id) {
    for (int i=0; i<players.size(); i++) {
        if (players[i].id == player_id) {
            players[i].get_rewards();
            return;
        }
    }
    throw std::invalid_argument("get_rewards: Player id not found");
}

void Game_State::take_tribe_at_position(int position, int player_id) {
    for (int i=0; i<players.size(); i++) {
        if (players[i].id == player_id) {
            Tribe* tribe = tribe_stack.take_tribe_at_position(position);
            players[i].set_active_tribe(tribe);
            return;
        }
    }
    throw std::invalid_argument("take_tribe_at_position: Player id not found");
}

void Game_State::go_in_decline(int player_id) {
    for (int i=0; i<players.size(); i++) {
        if (players[i].id == player_id) {
            players[i].go_in_decline();
            return;
        }
    }
    throw std::invalid_argument("go_in_decline: Player id not found");
}

std::vector<Tribe*> Game_State::get_tribes_on_top() {
    return tribe_stack.get_tribes_on_top();
}

void Game_State::abandon_area(){}

void Game_State::next_round () {
    round++;
}

Map& Game_State::get_map() {
    return map;
}

Tribe_Stack& Game_State::get_tribe_stack() {
    return tribe_stack;
}

Player& Game_State::get_current_player() {
    return *current_player;
}

}

