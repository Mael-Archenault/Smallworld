#include <unordered_set>

#include "effects.h"
#include "state.h"

namespace effects
{

Seafaring_Power::Seafaring_Power() : Power_Description("Seafaring", 5) {}

std::vector<std::pair<int, int>> Seafaring_Power::conquest_prices_effect(
    std::vector<std::pair<int, int>> initial_conquest_price, std::vector<state::Area*> owned_areas,
    state::Map* map)
{
    std::unordered_set<int> already_present;
    for (const auto& p : initial_conquest_price) already_present.insert(p.first);

    std::vector<std::pair<int, int>> result = initial_conquest_price;

    if (owned_areas.empty())
    {
        state::Tribe dummy_tribe(0, new Species_Description("DummySpecies", 0, 0),
                                 (Power_Description*) this);

        auto starting_points = map->get_starting_points_prices(dummy_tribe, true);

        for (auto& price_info : starting_points)
        {
            state::Area& area = map->get_area(price_info.first);
            if (area.get_biome() == state::Area_Biome::WATER)
            {
                if (!already_present.count(price_info.first))
                {
                    result.push_back(price_info);
                    already_present.insert(price_info.first);
                }
            }
        }

        return result;
    }

    for (state::Area* area : owned_areas)
    {
        if (!area) continue;

        for (state::Area* neighbor : area->get_neighbors())
        {
            if (!neighbor) continue;
            if ((neighbor->get_owner_tribe() != nullptr) &&
                (neighbor->get_owner_tribe() == area->get_owner_tribe()))
                continue;

            int nid = neighbor->id;
            if (already_present.count(nid)) continue;

            int price = neighbor->get_conquest_price(*area->get_owner_tribe());
            result.emplace_back(nid, price);
            already_present.insert(nid);
        }
    }

    return result;
}

}  // namespace effects
