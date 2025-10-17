#include "Game_State.h"

namespace state {

Game_State::Game_State(int n_players) : n_players(n_players), round(0), map(Map()){
    for (int i=0; i<n_players; i++ ) {
        players.push_back(new Player(i));
    }
}

void Game_State::gather_free_units(int player_id, int tribe_id) {
    for (auto* player : players) {
        if (player->id == player_id) {
            player->gather_free_units(tribe_id);
            break;
        }
    }
}

int Game_State::get_free_units_number(int player_id, int tribe_id) {
    int result;
    for (auto* player : players) {
        if (player->id == player_id) {
            result =player->get_free_units_number(tribe_id);
            break;
        }
    }
    return result;
}

std::vector<std::vector<int>> Game_State::get_conquest_prices(int player_id, int tribe_id) {
    std::vector<std::vector<int>> result;
    for (auto* player : players) {
        if (player->id == player_id) {
            result = player->get_conquest_prices(tribe_id);
            break;
        }
    }
    return result;
}

void Game_State::conquer(int attacking_player_id, int attacking_tribe_id, int attacked_area_id, int n_units, int dice_units) {
    Area& attacked_area = map.get_area(attacked_area_id);
    for (auto* player : players) {
        if (player->id == attacking_player_id) {
            player->conquer(attacking_tribe_id, attacked_area, n_units, dice_units);
            break;
        }
    }
}

int Game_State::roll_dice_for_bonus_units() {
    std::vector<int> possible_values = {0,0,0,1,2,3};

    int random_index = std::rand()%5;

    return possible_values[random_index];
}

void Game_State::redeploy_units(int player_id, int tribe_id, int area_id, int n_added_units) {
}

void Game_State::get_rewards(int player_id) {
    for (auto* player : players) {
        if (player->id == player_id) {
            player->get_rewards();
            break;
        }
    }
}

void Game_State::take_tribe_at_position(int position, int player_id) {
    for (auto* player : players) {
        if (player->id == player_id) {
            Tribe tribe = tribe_stack.take_tribe_at_position(position);
            player->set_active_tribe(tribe);
            break;
        }
    }
}

void Game_State::go_in_decline(int player_id) {
    for (auto* player : players) {
        if (player->id == player_id) {
            player->go_in_decline();
            break;
        }
    }
}

std::vector<Tribe*> Game_State::get_tribes_on_top() {
    return tribe_stack.get_tribes_on_top();
}

void Game_State::abandon_area(){}

void Game_State::next_round () {
    round++;
}

}

