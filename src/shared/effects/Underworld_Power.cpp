#include "effects.h"
#include <algorithm>
#include "state.h"

namespace effects
{

Mounted_Power::Mounted_Power() : Power_Description("Underworld", 5)
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
    std::vector<std::pair<int,int>> initial_conquest_price,
    std::vector<state::Area*> owned_areas,
    state::Map* map)
{
    if (effect_disabled)
        return initial_conquest_price;

    std::vector<int> reduced_price_area_ids;
    for (auto& price_info : initial_conquest_price)
    {
        int area_id = price_info.first;
        state::Area* area = nullptr;
        if (map)
            area = &map->get_area(area_id);
        else
        {
            for (auto* a : owned_areas)
            {
                if (a && a->id == area_id)
                {
                    area = a;
                    break;
                }
            }
        }
        if (!area) continue;

        auto biome = area->get_biome();
        if (biome == state::Area_Specialization::CAVERN)
            reduced_price_area_ids.push_back(area_id);
    }

    std::vector<std::pair<int,int>> result = initial_conquest_price;
    for (int area_id : reduced_price_area_ids)
    {
        for (auto& price_info : initial_conquest_price)
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
