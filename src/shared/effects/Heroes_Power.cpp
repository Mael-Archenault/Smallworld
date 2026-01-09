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



Heroes_Power::get_nb_of_special_tokens()
{
    return number_of_heroes;
}
}