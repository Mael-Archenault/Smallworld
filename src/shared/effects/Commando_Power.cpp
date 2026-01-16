#include "effects.h"
#include <algorithm>
#include "state.h"

namespace effects
{
    Commando_Power::Commando_Power ()
        : Power_Description("Commando",4)
    {
    }

std::vector<std::pair<int,int>> modify_price(
    std::vector<std::pair<int,int>> prices,
    int area_id,
    int price); // defined in giants_species.cpp


std::vector<std::pair<int,int>> Commando_Power::conquest_prices_effect(
    std::vector<std::pair<int,int>> initial_conquest_price,
    std::vector<state::Area*> owned_areas,
    state::Map* map)
{
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
}