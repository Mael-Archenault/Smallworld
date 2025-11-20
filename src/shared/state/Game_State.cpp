#include "Game_State.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace state
{

Game_State::Game_State(int n_players) : n_players(n_players), round(0), map(Map("4_players"))
{
    for (int i = 0; i < n_players; i++)
    {
        players.push_back(Player(i));
    }
    current_player     = &players[0];
    current_turn_phase = Turn_Phase::START;
    tribe_stack        = Tribe_Stack();
}

void Game_State::gather_free_units(int player_id)
{
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i].id == player_id)
        {
            players[i].gather_free_units();
            return;
        }
    }
    throw std::invalid_argument("gather_free_units: Player id not found");
}

int Game_State::get_free_units_number(int player_id)
{
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i].id == player_id)
        {
            return players[i].get_free_units_number();
        }
    }
    throw std::invalid_argument("get_free_units_number: Player id not found");
}

std::vector<std::pair<int, int>> Game_State::get_conquest_prices(int player_id)
{
    std::vector<std::vector<int>> result;
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i].id == player_id)
        {
            return players[i].get_conquest_prices(&map);
        }
    }
    throw std::invalid_argument("get_conquest_prices: Player id not found");
}

std::vector<int> Game_State::get_redeployable_areas(int player_id)
{
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i].id == player_id)
        {
            return players[i].get_redeployable_areas();
        }
    }
    throw std::invalid_argument("get_redeployable_areas: Player id not found");
}

void Game_State::conquer(int attacking_player_id, int attacked_area_id, int n_units, int dice_units)
{
    std::vector<std::pair<int, int>> attackable_areas = get_conquest_prices(attacking_player_id);
    std::vector<int>                 area_ids;
    for (size_t i = 0; i < attackable_areas.size(); i++)
    {
        area_ids.push_back(attackable_areas.at(i).first);
    }
    if (std::find(area_ids.begin(), area_ids.end(), attacked_area_id) == area_ids.end())
    {
        throw std::invalid_argument("conquer: Area not conquerable by the player");
    }
    Area& attacked_area = map.get_area(attacked_area_id);
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i].id == attacking_player_id)
        {
            players[i].conquer(&attacked_area, n_units, dice_units);
            return;
        }
    }
    throw std::invalid_argument("conquer: Player id not found");
}

int Game_State::roll_dice_for_bonus_units()
{
    std::vector<int> possible_values = {0, 0, 0, 1, 2, 3};

    int random_index = std::rand() % 5;

    return possible_values[random_index];
}

void Game_State::redeploy_units(int player_id, int area_id, int n_added_units)
{
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i].id == player_id)
        {
            players[i].redeploy_units(area_id, n_added_units);
            return;
        }
    }
    throw std::invalid_argument("redeploy_units: Player id not found");
}

void Game_State::get_rewards(int player_id)
{
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i].id == player_id)
        {
            players[i].get_rewards();
            return;
        }
    }
    throw std::invalid_argument("get_rewards: Player id not found");
}

void Game_State::take_tribe_at_position(int position, int player_id)
{
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i].id == player_id)
        {
            Tribe* tribe = tribe_stack.take_tribe_at_position(position);
            tribe->set_owner(&players[i]);
            players[i].set_active_tribe(tribe, position);
            return;
        }
    }
    throw std::invalid_argument("take_tribe_at_position: Player id not found");
}

void Game_State::go_in_decline(int player_id)
{
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i].id == player_id)
        {
            players[i].go_in_decline();
            return;
        }
    }
    throw std::invalid_argument("go_in_decline: Player id not found");
}

std::vector<Tribe*> Game_State::get_tribes_on_top()
{
    return tribe_stack.get_tribes_on_top();
}

void Game_State::abandon_area() {}

void Game_State::next_round()
{
    round++;
}

Map& Game_State::get_map()
{
    return map;
}

Tribe_Stack& Game_State::get_tribe_stack()
{
    return tribe_stack;
}

Player& Game_State::get_current_player()
{
    return *current_player;
}

Turn_Phase Game_State::get_current_turn_phase()
{
    return current_turn_phase;
}

void Game_State::set_current_turn_phase(Turn_Phase phase)
{
    current_turn_phase = phase;

    std::string phase_name;
    if (phase == Turn_Phase::START)
    {
        phase_name = "START";
    }
    else if (phase == Turn_Phase::CONQUER)
    {
        phase_name = "CONQUER";
    }
    else if (phase == Turn_Phase::REDEPLOY)
    {
        phase_name = "REDEPLOY";
    }

    std::cout << "Current phase set to : " + phase_name << std::endl;
}

void Game_State::next_player()
{
    int current_index = current_player->id;
    int next_index    = (current_index + 1) % n_players;
    current_player    = &players.at(next_index);
}
}  // namespace state
