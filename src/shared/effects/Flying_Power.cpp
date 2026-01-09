#include "effects.h"
#include "state.h"

namespace effects
{
    Flying_Power::Flying_Power()
    : Power_Description("Flying", 5)
{
}

std::vector<std::pair<int, int>> Flying_Power::conquest_prices_effect(
    std::vector<std::pair<int, int>> initial_conquest_prices,
    std::vector<state::Area*> owned_areas,
    state::Map* map)
{
    if (!map)
        return initial_conquest_prices;

    state::Tribe dummy_tribe(0, nullptr, new Power_Description("Flying", 0));
    std::vector<std::pair<int, int>> starting_points_prices =
        map->get_starting_points_prices(dummy_tribe, true);

    std::vector<std::pair<int, int>> result;
    for (auto& price_info : starting_points_prices)
    {
        state::Area& area = map->get_area(price_info.first);
        if (area.get_biome() != state::Area_Biome::WATER)
            result.push_back(price_info);
    }

    return result;
}
}
