#include "effects/Fortified_Power.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include "state/Area.h"

const int max_number_of_fortress = 6;

namespace effects
{
Fortified_Power::Fortified_Power() : Power_Description("Fortified", 3)
{
    number_of_fortress = 0;
}

void Fortified_Power::redeploy_effect(std::vector<state::Area*> owned_areas, int area_id, int n_added_tokens)
{
    if(number_of_fortress<max_number_of_fortress)
    {
        for (state::Area* area : owned_areas)
        {
            if (area->id == area_id)
            {
                if(std::find(area->get_special_tokens().begin(), area->get_special_tokens().end(),
                      state::Area_Special_Token::FORTRESS) != area->get_special_tokens().end())
                {
                    area->add_special_token(state::Area_Special_Token::FORTRESS);
                    number_of_fortress++;
                    return;
                }
                throw std::invalid_argument("there is already a FORTRESS on this area");
            }
        }
        throw std::invalid_argument("This area_id doesn't match an owned area for this tribe");
    }
}

int Fortified_Power::rewards_effect(std::vector<state::Area*> owned_areas)
{
    return number_of_fortress;
}

int Fortified_Power::get_nb_of_special_tokens()
{
    return number_of_fortress;
}

}