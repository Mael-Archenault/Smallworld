#include "state.h"

namespace state
{

Trolls_Species::Trolls_Species() : Species_Description("Trolls", 5, 10) {}

void Trolls_Species::apply_additional_defense(Area* area)
{
    for (Area_Special_Token special_tokens : area->get_special_tokens())
    {
        if (special_tokens == Area_Special_Token::TROLL_LAIR)
        {
            return;
        }
    }
    area->add_special_token(Area_Special_Token::TROLL_LAIR);
}

void Trolls_Species::disable_effects(std::vector<Area*> areas)
{
    for (Area* area : areas)
    {
        std::vector<Area_Special_Token>& tokens      = area->get_special_tokens();
        std::vector<Area_Special_Token>  tokens_copy = tokens;
        for (Area_Special_Token token : tokens)
        {
            // if (token == TROLL_LAIR)
            // {
            //     area->remove_special_token(Area_Special_Token::TROLL_LAIR);
            //     break;
            // }
        }
    }
}
}  // namespace state