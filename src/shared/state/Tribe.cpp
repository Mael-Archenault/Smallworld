#include "Tribe.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <unordered_set>

#include "Area_Biome.h"
#include "Game_State.h"
#include "Map.h"
#include "effects.h"

namespace state
{

Tribe::Tribe(int id, effects::Species_Description* base_species_description,
             effects::Power_Description* base_power_description)
    : id(id),
      species_description(base_species_description),
      power_description(base_power_description),
      owner(nullptr)
{
    owned_areas   = std::vector<Area*>();
    is_in_decline = false;

    free_units_number = 0;
    if (species_description != nullptr)
    {
        free_units_number += species_description->get_initial_units_number();
    }
    if (power_description != nullptr)
    {
        free_units_number += power_description->get_initial_units_number();
    }
}

effects::Species_Description* Tribe::get_species_description()
{
    return species_description;
}

effects::Power_Description* Tribe::get_power_description()
{
    return power_description;
}

void Tribe::gather_free_units(Turn_Phase turn_phase)
{
    for (size_t i = 0; i < owned_areas.size(); i++)
    {
        // Remove all units except one from each area and add them to free_units_number
        free_units_number += owned_areas.at(i)->gather_free_units();
    }
    // apply effect
    if (turn_phase == Turn_Phase::CONQUER)
    {
        free_units_number = species_description->first_gather_effect(free_units_number);
        free_units_number = power_description->first_gather_effect(free_units_number);
    }
    else if (turn_phase == Turn_Phase::REDEPLOY)
    {
        free_units_number = species_description->second_gather_effect(free_units_number);
        free_units_number = power_description->second_gather_effect(free_units_number);

    }
}

int Tribe::get_free_units_number()
{
    return free_units_number;
}

std::vector<std::pair<int, int>> Tribe::get_conquest_prices(Map* map)
{
    std::vector<std::pair<int, int>> prices;
    if (owned_areas.size() == 0)
    {
        prices = map->get_starting_points_prices(*this, false);
    }
    else
    {
        std::unordered_set<int> seen;
        prices.reserve(owned_areas.size() * 3);

        for (Area* area : owned_areas)
        {
            if (!area) continue;
            for (Area* neighbor : area->get_neighbors())
            {
                if (!neighbor) continue;
                if (neighbor->get_owner_tribe() == this) continue;
                int nid = neighbor->id;
                if (neighbor->get_biome() == Area_Biome::WATER) continue;
                if (seen.find(nid) != seen.end()) continue;
                seen.insert(nid);
                int price = neighbor->get_conquest_price(*this);
                prices.push_back(std::pair<int, int>{nid, price});
            }
        }
    }

    prices = species_description->conquest_prices_effect(prices, owned_areas, map);
    prices = power_description->conquest_prices_effect(prices, owned_areas, map);
    return prices;
}

std::vector<int> Tribe::get_owned_areas()
{
    std::vector<int> area_ids;
    area_ids.reserve(owned_areas.size());
    for (Area* area : owned_areas)
    {
        area_ids.push_back(area->id);
    }
    return area_ids;
}

std::vector<int> Tribe::get_redeployable_areas()
{
    return get_owned_areas();
}

void Tribe::redeploy_units(int area_id, int n_added_units)
{
    if (n_added_units > free_units_number)
    {
        throw std::invalid_argument("redeploy_units: not enough free units to redeploy.");
    }
    // Find the area by ID
    for (size_t i = 0; i < owned_areas.size(); i++)
    {
        if (owned_areas.at(i)->id == area_id)
        {
            owned_areas.at(i)->deploy_units(n_added_units);
            free_units_number -= n_added_units;
            return;
        }
    }
    throw std::invalid_argument("redeploy_units: area_id not found in owned_areas");
}

void Tribe::redeploy_tokens(int area_id, int n_added_tokens)
{  
    if(n_added_tokens > power_description->get_nb_of_special_tokens()){
        return;
    }
    species_description->redeploy_effect(owned_areas, area_id, n_added_tokens);
    power_description->redeploy_effect(owned_areas, area_id, n_added_tokens);
}

void Tribe::conquer(Area* attacked_area, int n_units, int dice_units, Map* map)
{
    // getting price for attacked area (with effects)
    std::pair<int, int> price_info =
        std::make_pair(attacked_area->id, attacked_area->get_conquest_price(*this));
    price_info =
        species_description
            ->conquest_prices_effect(std::vector<std::pair<int, int>>{price_info}, owned_areas, map)
            .at(0);
    price_info =
        power_description
            ->conquest_prices_effect(std::vector<std::pair<int, int>>{price_info}, owned_areas, map)
            .at(0);
    int needed_units = price_info.second;
    if (dice_units != -1)
    {
        if (n_units + dice_units < needed_units)
        {
            return;
        }
        else
        {
            power_description->conquest_effect(attacked_area);
            species_description->conquest_effect(attacked_area);
            attacked_area->change_owner(this);
            attacked_area->set_units_number(n_units);
            free_units_number -= n_units;
            owned_areas.push_back(attacked_area);

            return;
        }
    }
    if (n_units < needed_units)
    {
        throw std::invalid_argument("Tribe : conquer: not enough units to conquer the area");
    }
    if (n_units > free_units_number + std::max(0, dice_units))
    {
        throw std::invalid_argument("Tribe : conquer: not enough free units to conquer the area");
    }
    power_description->conquest_effect(attacked_area);
    species_description->conquest_effect(attacked_area);
    attacked_area->change_owner(this);
    attacked_area->set_units_number(n_units);
    free_units_number -= n_units;
    owned_areas.push_back(attacked_area);
}

void Tribe::go_in_decline()
{
    is_in_decline = true;
    species_description->decline_effect(owned_areas);
    power_description->decline_effect(owned_areas);

    // removing pawns from owned_areas
    for (Area* area : owned_areas)
    {
        area->gather_free_units();
    }
}

void Tribe::set_is_in_decline(bool new_state)
{
    is_in_decline = new_state;
}
int Tribe::get_rewards()
{
    int total_rewards = owned_areas.size() + species_description->rewards_effect(owned_areas) +
                        power_description->rewards_effect(owned_areas);
    return total_rewards;
}

std::string Tribe::get_species_name()
{
    return species_description->get_name();
}
std::string Tribe::get_power_name()
{
    return power_description->get_name();
}

bool Tribe::get_is_in_decline()
{
    return is_in_decline;
}

void Tribe::remove_from_map()
{
    power_description->disappearing_effect(owned_areas);
    species_description->disappearing_effect(owned_areas);
    std::vector<Area*> copy(owned_areas);
    for (auto& area : copy)
    {
        area->change_owner(nullptr);
        area->clear_units();
    }
}
void Tribe::remove_from_owned_areas(Area* area)
{
    if (std::find(owned_areas.begin(), owned_areas.end(), area) == owned_areas.end())
    {
        throw std::invalid_argument("Tribe : remove_from_owned_areas: area not owned by the tribe");
    }
    owned_areas.erase(std::find(owned_areas.begin(), owned_areas.end(), area));
}

void Tribe::gather_units_after_losing(Area* on_area)
{
    int gathered_units = on_area->get_units_number();
    free_units_number += gathered_units - 1;

    free_units_number += power_description->lose_effect(on_area);
    free_units_number += species_description->lose_effect(on_area);
}

Player* Tribe::get_owner()
{
    return owner;
}

void Tribe::set_owner(Player* player)
{
    owner = player;
}

void Tribe::set_species_description(effects::Species_Description* species_desc)
{
    species_description = species_desc;
}

void Tribe::set_power_description(effects::Power_Description* power_desc)
{
    power_description = power_desc;
}

void Tribe::set_free_units_number(int units_number)
{
    free_units_number = units_number;
}

void Tribe::add_to_owned_areas(Area* area)
{
    owned_areas.push_back(area);
}

Tribe Tribe::deep_copy()
{
    Tribe copy(id, nullptr, nullptr);
    copy.free_units_number = free_units_number;
    copy.is_in_decline     = is_in_decline;
    copy.owner             = nullptr;
    return copy;
}

void Tribe::to_json(Json::Value& root)
{
    root["id"]                       = id;
    root["is_in_decline"]            = is_in_decline;
    root["free_units_number"]        = free_units_number;
    root["species_description_name"] = species_description->get_name();
    root["power_description_name"]   = power_description->get_name();

    Json::Value owned_areas_json(Json::arrayValue);
    for (Area* area : owned_areas)
    {
        owned_areas_json.append(area->id);
    }
    root["owned_areas"] = owned_areas_json;
}

void Tribe::from_json(Json::Value& root)
{
    id                = root["id"].asInt();
    is_in_decline     = root["is_in_decline"].asBool();
    free_units_number = root["free_units_number"].asInt();

    owned_areas.clear();
}

}  // namespace state