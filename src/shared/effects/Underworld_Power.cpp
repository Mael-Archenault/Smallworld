#include "effects.h"
#include <algorithm>
#include "state.h"

namespace effects
{

Underworld_Power::Underworld_Power() 
    : Power_Description("Underworld", 5)
{
    effect_disabled = false;
}

std::vector<std::pair<int,int>> modify_price(
    std::vector<std::pair<int,int>> prices,
    int area_id,
    int price); // defined in giants_species.cpp

std::vector<std::pair<int,int>> Underworld_Power::conquest_prices_effect(
    std::vector<std::pair<int,int>> initial_conquest_price,
    std::vector<state::Area*> owned_areas,
    state::Map* map)
{
    if (effect_disabled)
        return initial_conquest_price;

    std::vector<int> reduced_price_area_ids;

    for (auto* area : owned_areas)  
    {
        if (!area) continue;
        const auto& specs = area->get_area_specialization();
        if (std::find(specs.begin(), specs.end(),
                      state::Area_Specialization::CAVERN) != specs.end())
        {
            reduced_price_area_ids.push_back(area->id); 
        }
    }

    std::vector<std::pair<int,int>> result = initial_conquest_price;

    if (!reduced_price_area_ids.empty() && map)  
    {
        state::Tribe dummy_tribe(0, (Species_Description*) nullptr, (Power_Description*) this); 
        std::vector<std::pair<int,int>> all_prices =
            map->get_starting_points_prices(dummy_tribe, true); 

        for (auto& p : all_prices) 
        {
            state::Area& area = map->get_area(p.first); 
            const auto& specs = area.get_area_specialization(); 
            if (std::find(specs.begin(), specs.end(),
                          state::Area_Specialization::CAVERN) != specs.end() &&
                area.get_biome() != state::Area_Biome::WATER) 
            {
                auto it = std::find_if(result.begin(), result.end(),
                                       [&](auto& rp){ return rp.first == area.id; }); 
                if (it == result.end()) 
                {
                    result.push_back({area.id, p.second}); 
                }
            }
        }
    }

    for (auto* area : owned_areas)
    {
        if (!area) continue;
        const auto& specs = area->get_area_specialization();
        if (std::find(specs.begin(), specs.end(),
                      state::Area_Specialization::CAVERN) != specs.end())
        {
            auto it = std::find_if(result.begin(), result.end(),
                                   [&](auto& p){ return p.first == area->id; });
            if (it != result.end())
            {
                it->second = std::max(1, it->second - 1); 
            }
        }
    }

    if (map)
    {
        for (auto& price_info : result)
        {
            state::Area& area = map->get_area(price_info.first);
            const auto& specs = area.get_area_specialization();
            if (std::find(specs.begin(), specs.end(),
                          state::Area_Specialization::CAVERN) != specs.end())
            {
                price_info.second = std::max(1, price_info.second - 1);
            }
        }
    }

    for (int area_id : reduced_price_area_ids)
    {
        for (auto& price_info : result)
        {
            if (price_info.first == area_id)
            {
                result = modify_price(result, area_id, price_info.second);
            }
        }
    }

    return result;
}

void Underworld_Power::decline_effect(std::vector<state::Area*> owned_areas)
{
    effect_disabled = true;
}

} // namespace effects
