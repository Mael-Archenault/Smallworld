#include "Tribe.h"

#include <algorithm>
#include <stdexcept>
#include <unordered_set>

#include "Area_Biome.h"
#include "Effects_Bundle.h"
#include "Game_State.h"
#include "Map.h"
#include "Power_Description.h"
#include "Species_Description.h"

using namespace state;

Tribe::Tribe(int id, Species_Description* species_description, Power_Description* power_description)
    : id(id),
      species_description(species_description),
      power_description(power_description),
      owner(nullptr)
{
    owned_areas       = std::vector<Area*>();
    in_decline        = false;
    free_units_number = species_description->get_initial_units_number() +
                        power_description->get_initial_units_number();
}

Species_Description* Tribe::get_species_description()
{
    return species_description;
}

Power_Description* Tribe::get_power_description()
{
    return power_description;
}

void Tribe::gather_free_units()
{
    for (size_t i = 0; i < owned_areas.size(); i++)
    {
        // Remove all units except one from each area and add them to free_units_number
        free_units_number += owned_areas[i]->gather_free_units();
    }
    free_units_number +=
        species_description->add_free_units(free_units_number + owned_areas.size());
}

int Tribe::get_free_units_number()
{
    return free_units_number;
}

std::vector<std::pair<int, int>> Tribe::get_conquest_prices(Map* map)
{
    if (owned_areas.size() == 0)
    {
        return map->get_starting_points_prices(*this,
                                               false /*species_description.can_start_anywhere();*/);
    }

    std::vector<std::pair<int, int>> prices;
    std::unordered_set<int>          seen;
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
    return prices;
}

std::vector<int> Tribe::get_redeployable_areas()
{
    std::vector<int> area_ids;
    area_ids.reserve(owned_areas.size());
    for (Area* area : owned_areas)
    {
        area_ids.push_back(area->id);
    }
    return area_ids;
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
        if (owned_areas[i]->id == area_id)
        {
            owned_areas[i]->deploy_units(n_added_units);
            species_description->apply_additional_defense(owned_areas[i]);
            power_description->apply_additional_defense(owned_areas[i]);
            free_units_number -= n_added_units;
            return;
        }
    }
    throw std::invalid_argument("redeploy_units: area_id not found in owned_areas");
}

void Tribe::conquer(Area* attacked_area, int n_units, int dice_units)
{
    if (dice_units != -1)
    {
        if (n_units + dice_units < attacked_area->get_conquest_price(*this))
        {
            return;
        }
        else
        {
            attacked_area->set_owner_tribe(this);
            attacked_area->set_units_number(n_units);
            species_description->areas_conquered(attacked_area);
            free_units_number -= n_units;
            owned_areas.push_back(attacked_area);
            return;
        }
    }
    if (n_units < attacked_area->get_conquest_price(*this))
    {
        throw std::invalid_argument("Tribe : conquer: not enough units to conquer the area");
    }
    if (n_units > free_units_number + std::max(0, dice_units))
    {
        throw std::invalid_argument("Tribe : conquer: not enough free units to conquer the area");
    }
    attacked_area->set_owner_tribe(this);
    attacked_area->set_units_number(n_units);
    species_description->areas_conquered(attacked_area);
    free_units_number -= n_units;
    owned_areas.push_back(attacked_area);
}

void Tribe::go_in_decline()
{
    in_decline = true;
}
int Tribe::get_rewards()
{
    int reward = owned_areas.size() + species_description->get_bonus_rewards(owned_areas) +
                 power_description->get_bonus_rewards(owned_areas);
    return reward;
}

std::string Tribe::get_species_name()
{
    return species_description->get_name();
}
std::string Tribe::get_power_name()
{
    return power_description->get_name();
}

bool Tribe::is_in_decline()
{
    return in_decline;
}

void Tribe::remove_from_map()
{
    std::vector<Area*> copy(owned_areas);
    for (auto& area : copy)
    {
        area->set_owner_tribe(nullptr);
        area->clear_units();
    }
}
void Tribe::remove_from_owned_areas(Area* area)
{
    if (int i = std::find_if(owned_areas.begin(), owned_areas.end(),
        [area](Area* area_to_test) {return area_to_test->id == area->id;}) != owned_areas.end())
    {
        owned_areas.erase(owned_areas.begin() + i);
        return;

    }
    throw std::invalid_argument("Tribe : remove_from_owned_areas: area not owned by the tribe");
}

void Tribe::gather_units_after_losing(Area* on_area)
{
    int gathered_units = on_area->get_units_number();
    free_units_number += gathered_units - 1;
}

Player* Tribe::get_owner()
{
    return owner;
}

void Tribe::set_owner(Player* player)
{
    owner = player;
}