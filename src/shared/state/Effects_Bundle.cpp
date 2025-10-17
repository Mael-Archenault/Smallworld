//
// Created by vboxuser on 10/17/25.
//

#include "Effects_Bundle.h"

state::Effects_Bundle::Effects_Bundle() = default;
void state::Effects_Bundle::apply_first_round_effect (){}
void state::Effects_Bundle::apply_conquest_effect (){}
void state::Effects_Bundle::apply_lose_effect (){}
int state::Effects_Bundle::get_bonus_rewards (state::Area& area){return 0;}