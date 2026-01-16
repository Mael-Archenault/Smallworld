#include "Player.h"

#include <iostream>
#include <stdexcept>

namespace state
{

Player::Player(int id, std::string name) : id(id), name(name)
{
    money            = 5;
    active_tribe     = nullptr;
    tribe_in_decline = nullptr;
}

Player::~Player() {}

void Player::gather_free_units(Turn_Phase turn_phase)
{
    if (active_tribe != nullptr)
    {
        active_tribe->gather_free_units(turn_phase);
        return;
    }
    throw std::invalid_argument("gather_free_units:there is no active tribe for this player");
}

int Player::get_free_units_number()
{
    if (active_tribe != nullptr)
    {
        return active_tribe->get_free_units_number();
    }
    throw std::invalid_argument("get_free_units_number:there is no active tribe for this player");
}

std::vector<std::pair<int, int>> Player::get_conquest_prices(Map* map)
{
    if (active_tribe != nullptr)
    {
        return active_tribe->get_conquest_prices(map);
    }
    throw std::invalid_argument("get_conquest_prices:there is no active tribe for this player");
}

std::vector<int> Player::get_redeployable_areas()
{
    if (active_tribe != nullptr)
    {
        return active_tribe->get_redeployable_areas();
    }
    throw std::invalid_argument("get_redeployable_areas:there is no active tribe for this player");
}

void Player::redeploy_units(int area_id, int n_added_units)
{
    if (active_tribe != nullptr)
    {
        active_tribe->redeploy_units(area_id, n_added_units);
        return;
    }
    throw std::invalid_argument("redeploy_units:there is no active tribe for this player");
}

void Player::redeploy_tokens(int area_id, int n_added_tokens)
{
    if (active_tribe != nullptr)
    {
        active_tribe->redeploy_tokens(area_id, n_added_tokens);
        return;
    }
    throw std::invalid_argument("redeploy_tokens:there is no active tribe for this player");
}

void Player::conquer(Area* attacked_area, int n_units, int dice_units, Map* map)
{
    if (active_tribe != nullptr)
    {
        active_tribe->conquer(attacked_area, n_units, dice_units, map);
        return;
    }
    throw std::invalid_argument("conquer:there is no active tribe for this player");
}

void Player::get_rewards()
{
    if (active_tribe != nullptr)
    {
        money += active_tribe->get_rewards();
    }

    if (tribe_in_decline != nullptr)
    {
        money += tribe_in_decline->get_rewards();
    }
}

void Player::go_in_decline()
{
    if (active_tribe != nullptr)
    {
        active_tribe->go_in_decline();
        if (tribe_in_decline != nullptr)
        {
            tribe_in_decline->remove_from_map();
        }
        tribe_in_decline = active_tribe;
        active_tribe     = nullptr;
        return;
    }
    throw std::invalid_argument("go_in_decline:there is no active tribe for this player");
}

void Player::choose_active_tribe(Tribe* tribe, int cost)
{
    if (money < cost)
    {
        throw std::invalid_argument("not enough money");
    }
    money -= cost;
    active_tribe = tribe;
}
void Player::set_active_tribe(Tribe* tribe)
{
    active_tribe = tribe;
}

void Player::set_in_decline_tribe(Tribe* tribe)
{
    tribe_in_decline = tribe;
}

std::pair<Tribe*, Tribe*> Player::get_tribes()
{
    return std::make_pair(active_tribe, tribe_in_decline);
}

int Player::get_money()
{
    return money;
}

std::string Player::get_name()
{
    return name;
}

void Player::set_money(int new_money)
{
    money = new_money;
}

void Player::to_json(Json::Value& root)
{
    root["id"]    = id;
    root["name"]  = name;
    root["money"] = money;

    if (active_tribe != nullptr)
    {
        root["active_tribe_id"] = active_tribe->id;
    }
    else
    {
        root["active_tribe_id"] = -1;
    }

    if (tribe_in_decline != nullptr)
    {
        root["tribe_in_decline_id"] = tribe_in_decline->id;
    }
    else
    {
        root["tribe_in_decline_id"] = -1;
    }
}

void Player::from_json(Json::Value& root)
{
    id    = root["id"].asInt();
    name  = root["name"].asString();
    money = root["money"].asInt();
}

}  // namespace state
