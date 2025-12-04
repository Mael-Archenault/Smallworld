#include "effects/Elves_Species.h"

#include "state/Area.h"

namespace effects
{
Elves_Species::Elves_Species() : Species_Description("Elves", 6, 11) {}

int Elves_Species::lose_effect(state::Area* lost_area)
{
    return 1;
}
}  // namespace effects