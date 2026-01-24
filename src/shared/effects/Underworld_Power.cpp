#include <algorithm>

#include "effects.h"
#include "state.h"

namespace effects
{

Underworld_Power::Underworld_Power() : Power_Description("Underworld", 5)
{
    effect_disabled = false;
}

std::vector<std::pair<int, int>> modify_price(std::vector<std::pair<int, int>> prices, int area_id,
                                              int price);  // defined in giants_species.cpp

std::vector<std::pair<int, int>> Underworld_Power::conquest_prices_effect(
    std::vector<std::pair<int, int>> initial_conquest_price, std::vector<state::Area*> owned_areas,
    state::Map* map)
{
    if (effect_disabled) return initial_conquest_price;

    for (auto& price : initial_conquest_price)
    {
        state::Area& area = map->get_area(price.first);
        bool         has_cavern =
            std::find(area.get_area_specialization().begin(), area.get_area_specialization().end(),
                      state::Area_Specialization::CAVERN) != area.get_area_specialization().end();
        if (has_cavern)
        {
            price.second = std::max(1, price.second - 1);
        }
    }

    bool owns_a_cavern = false;
    for (state::Area* area : owned_areas)
    {
        if (!area) continue;
        bool has_cavern =
            std::find(area->get_area_specialization().begin(),
                      area->get_area_specialization().end(),
                      state::Area_Specialization::CAVERN) != area->get_area_specialization().end();
        if (has_cavern)
        {
            owns_a_cavern = true;
            break;
        }
    }
    if (!owns_a_cavern)
    {
        return initial_conquest_price;
    }

    for (auto& area : map->get_areas())
    {
        bool has_cavern =
            std::find(area.get_area_specialization().begin(), area.get_area_specialization().end(),
                      state::Area_Specialization::CAVERN) != area.get_area_specialization().end();
        if (has_cavern)
        {
            initial_conquest_price.push_back(
                {area.id,
                 std::max(1, area.get_conquest_price(*owned_areas[0]->get_owner_tribe()) - 1)});
        }
    }

    return initial_conquest_price;
}

void Underworld_Power::decline_effect(std::vector<state::Area*> owned_areas)
{
    effect_disabled = true;
}

}  // namespace effects
