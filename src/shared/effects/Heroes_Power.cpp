#include "effects/Heroes_Power.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include "state/Area.h"

const int max_number_of_heroes = 2;

namespace effects
{
Heroes_Power::Heroes_Power() : Power_Description("Heroes", 5)
{
    number_of_heroes = 0;
}



int Heroes_Power::get_nb_of_special_tokens()
{
    return number_of_heroes;
}

void Heroes_Power::redeploy_effect(std::vector<state::Area*> owned_areas, int area_id, int n_added_tokens)
{
    if(number_of_heroes<max_number_of_heroes)
    {
        for (state::Area* area : owned_areas)
        {
            if (area->id == area_id)
            {
                if(std::find(area->get_special_tokens().begin(), area->get_special_tokens().end(),
                      state::Area_Special_Token::HERO) != area->get_special_tokens().end())
                {
                    area->add_special_token(state::Area_Special_Token::HERO);
                    number_of_heroes++;
                    return;
                }
                throw std::invalid_argument("there is already a FORTRESS on this area");
            }
        }
        throw std::invalid_argument("This area_id doesn't match an owned area for this tribe");
    }
}

void Heroes_Power::decline_effect(std::vector<state::Area*> owned_areas)
{
    effect_disabled = true;
    disappearing_effect();
}

void Heroes_Power::disappearing_effect(std::vector<state::Area*> owned_areas)
{
    number_of_heroes = 0;
    for (state::Area* area : owned_areas)
    {
        auto& tokens = area->get_special_tokens();
        auto  it = std::find(tokens.begin(), tokens.end(), state::Area_Special_Token::HERO);
        if (it != tokens.end())
        {
            tokens.erase(it);
        }
    }
}

}