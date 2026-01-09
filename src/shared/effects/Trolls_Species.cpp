#include "effects/Trolls_Species.h"

#include <algorithm>

#include "state/Area.h"

namespace effects
{

Trolls_Species::Trolls_Species() : Species_Description("Trolls", 5, 10)
{
    placed_troll_lairs = 0;
    max_troll_lairs    = 10;
}

void Trolls_Species::redeploy_effect(std::vector<state::Area*> owned_areas, int area_id, int n_added_tokens)
{
    // making sure each owned area has a troll lair
    for (state::Area* area : owned_areas)
    {
        if (placed_troll_lairs == max_troll_lairs)
        {
            return;
        }
        if (std::find(area->get_special_tokens().begin(), area->get_special_tokens().end(),
                      state::Area_Special_Token::TROLL_LAIR) != area->get_special_tokens().end())
        {
            continue;
        }
        area->add_special_token(state::Area_Special_Token::TROLL_LAIR);
        placed_troll_lairs++;
    }
}

int Trolls_Species::lose_effect(state::Area* lost_area)
{
    auto& tokens = lost_area->get_special_tokens();
    auto  it     = std::find(tokens.begin(), tokens.end(), state::Area_Special_Token::TROLL_LAIR);
    if (it != tokens.end())
    {
        tokens.erase(it);
    }
    placed_troll_lairs--;
    return 0;
}

void Trolls_Species::disappearing_effect(std::vector<state::Area*> owned_areas)
{
    for (state::Area* area : owned_areas)
    {
        auto& tokens = area->get_special_tokens();
        auto  it = std::find(tokens.begin(), tokens.end(), state::Area_Special_Token::TROLL_LAIR);
        if (it != tokens.end())
        {
            tokens.erase(it);
        }
    }
    placed_troll_lairs = 0;
}
}  // namespace effects