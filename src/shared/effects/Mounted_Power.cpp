#include "effects.h"
#include <algorithm>
#include "state.h"

namespace effects
{

Mounted_Power::Mounted_Power() : Power_Description("Mounted", 5)
{
    effect_disabled = false;
}

std::vector<std::pair<int,int>> modify_price(
    std::vector<std::pair<int,int>> prices,
    int area_id,
    int price)
{
    std::vector<std::pair<int,int>> modified_prices;
    for (auto& price_info : prices)
    {
        if (price_info.first == area_id)
            modified_prices.push_back({area_id, price});
        else
            modified_prices.push_back(price_info);
    }
    return modified_prices;
}

std::vector<std::pair<int,int>> Mounted_Power::conquest_prices_effect(
    std::vector<state::Area*> initial_areas,
    std::vector<state::Area*> owned_areas,
    state::Map* map)
{
    if (effect_disabled)
        return {};

    std::vector<std::pair<int,int>> initial_prices;
    for (auto* area : initial_areas)
    {
        if (!area) continue;
        initial_prices.push_back({area->id, area->get_units_number() + 1});
    }

    std::vector<int> reduced_price_area_ids;
    for (auto* area : initial_areas)
    {
        if (!area) continue;
        auto biome = area->get_biome();
        if (biome == state::Area_Biome::HILL || biome == state::Area_Biome::FARM)
        {
            reduced_price_area_ids.push_back(area->id);
        }
    }

    std::vector<std::pair<int,int>> result = initial_prices;
    for (int area_id : reduced_price_area_ids)
    {
        for (auto& price_info : initial_prices)
        {
            if (price_info.first == area_id)
            {
                result = modify_price(result, area_id, std::max(1, price_info.second - 1));
            }
        }
    }

    return result;
}

void Mounted_Power::decline_effect(std::vector<state::Area*> owned_areas)
{
    effect_disabled = true;
}

} // namespace effects
