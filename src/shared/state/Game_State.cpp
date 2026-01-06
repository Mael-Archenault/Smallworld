#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "state.h"

namespace state
{

Game_State::Game_State(int n_players, std::vector<std::string> names)
    : n_players(n_players),
      round(1),
      map("4_players"),
      names(names),
      tribe_stack(n_players),
      current_turn_phase(Turn_Phase::START)
{
    for (int i = 0; i < n_players; i++)
    {
        players.push_back(Player(i, names.at(i)));
    }
    current_player = &players[0];
}

void Game_State::gather_free_units(int player_id)
{
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i].id == player_id)
        {
            players[i].gather_free_units(current_turn_phase);
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
    // verifying if the area is attackable by the player

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

    // attacking the targeted area
    Area& attacked_area = map.get_area(attacked_area_id);
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i].id == attacking_player_id)
        {
            players[i].conquer(&attacked_area, n_units, dice_units, &map);
            return;
        }
    }
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
        if (players.at(i).id == player_id)
        {
            Tribe* tribe = tribe_stack.take_tribe_at_position(position);
            tribe->set_owner(&players[i]);
            players.at(i).choose_active_tribe(tribe, position);
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
            if (players.at(i).get_tribes().second != nullptr)
            {
                tribe_stack.remove_from_in_game_tribes(players.at(i).get_tribes().second->id);
            }
            players.at(i).go_in_decline();
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
}

void Game_State::next_player()
{
    int current_index = current_player->id;
    int next_index;
    if (current_index + 1 >= n_players)
    {
        next_round();
        next_index = 0;
    }
    else
    {
        next_index = current_index + 1;
    }
    current_player = &players.at(next_index);
}

bool Game_State::is_game_finished()
{
    return round > map.get_max_round();
}

std::vector<std::pair<int, int>> Game_State::get_all_player_id_money()
{
    std::vector<std::pair<int, int>> player_id_money;
    for (Player player : players)
    {
        player_id_money.emplace_back(player.id, player.get_money());
    }
    return player_id_money;
}

std::vector<Player>& Game_State::get_players()
{
    return players;
}

Game_State Game_State::deep_copy()
{
    Game_State copy(n_players, names);
    copy.round              = round;
    copy.current_turn_phase = current_turn_phase;
    copy.tribe_stack        = tribe_stack.deep_copy();
    copy.map                = map.deep_copy();

    // restoring owner tribe in areas
    for (Area& area : map.get_areas())
    {
        Area& area_copy = copy.map.get_area(area.id);
        if (area.get_owner_tribe() != nullptr)
        {
            std::vector<Tribe*> in_game_tribes = copy.tribe_stack.get_in_game_tribes();
            for (Tribe* tribe : in_game_tribes)
            {
                if (tribe->id == area.get_owner_tribe()->id)
                {
                    area_copy.set_owner_tribe(tribe);
                    tribe->add_to_owned_areas(&area_copy);
                }
            }
        }
    }

    // restoring current player

    copy.current_player = &copy.players.at(current_player->id);

    // restoring active and decline tribes pointers for each player
    for (size_t i = 0; i < players.size(); i++)
    {
        std::vector<Tribe*>       in_game_tribes = copy.tribe_stack.get_in_game_tribes();
        std::pair<Tribe*, Tribe*> tribes         = players.at(i).get_tribes();
        if (tribes.first != nullptr)
        {
            for (Tribe* tribe : in_game_tribes)
            {
                if (tribe->id == tribes.first->id)
                {
                    tribe->set_owner(&copy.players.at(i));
                    tribes.first = tribe;
                }
            }
            copy.players.at(i).set_active_tribe(tribes.first);
        }
        if (tribes.second != nullptr)
        {
            for (Tribe* tribe : in_game_tribes)
            {
                if (tribe->id == tribes.second->id)
                {
                    tribe->set_owner(&copy.players.at(i));
                    tribes.second = tribe;
                }
            }
        }
        copy.players.at(i).set_active_tribe(tribes.first);
        copy.players.at(i).set_in_decline_tribe(tribes.second);
    }

    return copy;
}
}  // namespace state
