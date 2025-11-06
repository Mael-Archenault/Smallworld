//
// Created by vboxuser on 10/17/25.
//

#include "Effects_Bundle.h"

using namespace state;

Effects_Bundle::Effects_Bundle() = default;
void Effects_Bundle::apply_first_round_effect (){}
void Effects_Bundle::apply_conquest_effect (Area* area){}
void Effects_Bundle::apply_lose_effect (){}
int Effects_Bundle::get_bonus_rewards (Area* area){return 0;}