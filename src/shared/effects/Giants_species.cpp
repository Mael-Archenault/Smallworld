
#include "effects/Giants_Species.h"

#include <algorithm>
#include <unordered_set>

#include "state/Area.h"
#include "state/Map.h"

namespace effects
{

std::vector<std::pair<int, int>> modify_price(std::vector<std::pair<int, int>> prices, int area_id,
                                              int price)
{
    std::vector<std::pair<int, int>> modified_prices;
    for (auto& price_info : prices)
    {
        if (price_info.first == area_id)
        {
            modified_prices.push_back(std::pair<int, int>{area_id, price});
        }
        else
        {
            modified_prices.push_back(price_info);
        }
    }
    return modified_prices;
}

Giants_Species::Giants_Species() : Species_Description("Giants", 6, 8) {}

std::vector<std::pair<int, int>> Giants_Species::conquest_prices_effect(
    std::vector<std::pair<int, int>> initial_conquest_prices, std::vector<state::Area*> owned_areas,
    state::Map* map)
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

            if (area->get_biome() == state::Area_Biome::MOUNTAINS)
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
