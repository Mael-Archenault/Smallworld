#include "effects/Halflings_Species.h"

#include <algorithm>

#include "state/Area.h"
#include "state/Map.h"
#include "state/Tribe.h"

namespace effects
{

Halflings_Species::Halflings_Species() : Species_Description("Halflings", 6, 8)
{
    holes_placed = 0;
}

void Halflings_Species::conquest_effect(state::Area* area)
{
    if (holes_placed < 2)
    {
        area->add_special_token(state::Area_Special_Token::HOLE_IN_THE_GROUND);
        holes_placed++;
    }
}
std::vector<std::pair<int, int>> Halflings_Species::conquest_prices_effect(
    std::vector<std::pair<int, int>> initial_conquest_prices, std::vector<state::Area*> owned_areas,
    state::Map* map)

{
    if (owned_areas.size() != 0)
    {
        return initial_conquest_prices;
    }
    // can start anywhere
    state::Tribe                     dummy_tribe(0, (Species_Description*) this,
                                                 new Power_Description("Power_name", 0));
    std::vector<std::pair<int, int>> starting_points_prices =
        map->get_starting_points_prices(dummy_tribe, true);

    // removing water zones

    std::vector<std::pair<int, int>> result;
    for (auto& price_info : starting_points_prices)
    {
        state::Area& area = map->get_area(price_info.first);
        if (area.get_biome() != state::Area_Biome::WATER)
        {
            result.push_back(price_info);
        }
    }
    return result;
}

void Halflings_Species::decline_effect(std::vector<state::Area*> owned_areas)
{
    holes_placed = 0;
    for (state::Area* area : owned_areas)
    {
        auto& tokens = area->get_special_tokens();
        auto  it =
            std::find(tokens.begin(), tokens.end(), state::Area_Special_Token::HOLE_IN_THE_GROUND);
        if (it != tokens.end())
        {
            tokens.erase(it);
        }
    }
}
}  // namespace effects