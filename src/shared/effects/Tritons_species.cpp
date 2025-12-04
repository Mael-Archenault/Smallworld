#include "effects/Tritons_Species.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

#include "state/Area.h"

namespace effects
{

std::vector<std::pair<int, int>> modify_price(std::vector<std::pair<int, int>> prices, int area_id,
                                              int price);  // defined in Giants_Species.cpp

bool is_coastal(state::Area* area)
{
    for (state::Area* neighbor : area->get_neighbors())
    {
        if (neighbor->get_biome() == state::Area_Biome::WATER)
        {
            std::cout << "coastal area found" << std::endl;
            return true;
        }
    }
    return false;
}

Tritons_Species::Tritons_Species() : Species_Description("Tritons", 6, 11) {}

std::vector<std::pair<int, int>> Tritons_Species::conquest_prices_effect(
    std::vector<std::pair<int, int>> initial_conquest_prices, std::vector<state::Area*> owned_areas)
{
    std::unordered_set<int> seen;

    state::Tribe*    effect_owner = nullptr;
    std::vector<int> reduced_price_area_ids;

    // getting all neighbors areas
    for (state::Area* area : owned_areas)
    {
        if (!area) continue;
        if (effect_owner == nullptr)
        {
            effect_owner = area->get_owner_tribe();
        }
        for (state::Area* neighbor : area->get_neighbors())
        {
            if (!neighbor) continue;
            if (neighbor->get_owner_tribe() == effect_owner) continue;
            if (seen.find(neighbor->id) != seen.end()) continue;

            if (is_coastal(neighbor))
            {
                reduced_price_area_ids.push_back(neighbor->id);
            }
        }
    }

    std::vector<std::pair<int, int>> result = initial_conquest_prices;
    for (int neighbor : reduced_price_area_ids)
    {
        for (auto& price_info : initial_conquest_prices)
        {
            if (price_info.first == neighbor)
            {
                result = modify_price(result, neighbor, std::max(1, price_info.second - 1));
            }
        }
    }

    return result;
}
}  // namespace effects