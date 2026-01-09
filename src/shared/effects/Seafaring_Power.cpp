#include "effects.h"
#include "state.h"
#include <unordered_set>

namespace effects
{

Seafaring_Power::Seafaring_Power()
    : Power_Description("Seafaring", 5)
{
}

std::vector<std::pair<int,int>> Seafaring_Power::conquest_prices_effect(
    std::vector<std::pair<int,int>> initial_conquest_price,
    std::vector<state::Area*> owned_areas,
    state::Map* map)
{
    std::unordered_set<int> already_present;
    for (const auto& p : initial_conquest_price)
        already_present.insert(p.first);

    std::vector<std::pair<int,int>> result = initial_conquest_price;

    for (state::Area* area : owned_areas)
    {
        if (!area) continue;

        for (state::Area* neighbor : area->get_neighbors())
        {
            if (!neighbor) continue;
            if (neighbor->get_owner_tribe() == area->get_owner_tribe())
                continue;

            int nid = neighbor->id;
            if (already_present.count(nid))
                continue;

            int price = neighbor->get_conquest_price(*area->get_owner_tribe());
            result.emplace_back(nid, price);
            already_present.insert(nid);
        }
    }

    return result;
}

} 
