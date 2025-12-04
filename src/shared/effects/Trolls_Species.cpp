#include "effects/Trolls_Species.h"

#include "state/Area.h"

namespace effects
{

Trolls_Species::Trolls_Species() : Species_Description("Trolls", 5, 10) {}

void Trolls_Species::apply_additional_defense(state::Area* area)
{
    for (state::Area_Special_Token special_tokens : area->get_special_tokens())
    {
        if (special_tokens == state::Area_Special_Token::TROLL_LAIR)
        {
            return;
        }
    }
    area->add_special_token(state::Area_Special_Token::TROLL_LAIR);
}

void Trolls_Species::disable_effects(std::vector<state::Area*> areas)
{
    for (state::Area* area : areas)
    {
        std::vector<state::Area_Special_Token>& tokens      = area->get_special_tokens();
        std::vector<state::Area_Special_Token>  tokens_copy = tokens;
        for (state::Area_Special_Token token : tokens)
        {
            // if (token == TROLL_LAIR)
            // {
            //     area->remove_special_token(Area_Special_Token::TROLL_LAIR);
            //     break;
            // }
        }
    }
}
}  // namespace effects