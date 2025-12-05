#include "effects/Tritons_Species.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

#include "state/Area.h"
#include "state/Map.h"

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
            return true;
        }
    }
    return false;
}

Tritons_Species::Tritons_Species() : Species_Description("Tritons", 6, 11) {}

std::vector<std::pair<int, int>> Tritons_Species::conquest_prices_effect(
    std::vector<std::pair<int, int>> initial_conquest_prices, std::vector<state::Area*> owned_areas,
    state::Map* map)
{
    std::vector<int> reduced_price_area_ids;

    for (auto& price_info : initial_conquest_prices)
    {
        if (is_coastal(&map->get_area(price_info.first)))
        {
            reduced_price_area_ids.push_back(price_info.first);
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