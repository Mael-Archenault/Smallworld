
#include "effects/Effects_Bundle.h"

#include "state/Area.h"

namespace effects
{

Effects_Bundle::Effects_Bundle() = default;

int Effects_Bundle::first_gather_effect(int initial_free_units_number)
{
    return initial_free_units_number;
}

std::vector<std::pair<int, int>> Effects_Bundle::conquest_prices_effect(
    std::vector<std::pair<int, int>> initial_conquest_price, std::vector<state::Area*> owned_areas)
{
    return initial_conquest_price;
}

void Effects_Bundle::conquest_effect(state::Area* area) {}

int Effects_Bundle::second_gather_effect(int initial_free_units_number)
{
    return initial_free_units_number;
}

void Effects_Bundle::redeploy_effect(std::vector<state::Area*> owned_areas) {}

int Effects_Bundle::rewards_effect(std::vector<state::Area*> owned_areas)
{
    return 0;
}
void Effects_Bundle::lose_effect(state::Area* lost_area) {}

void Effects_Bundle::decline_effect(std::vector<state::Area*> owned_areas) {}

void Effects_Bundle::disappearing_effect(std::vector<state::Area*> owned_areas) {}

Effects_Bundle::~Effects_Bundle() = default;
}  // namespace effects